module;

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <entt/entt.hpp>

module Gfx.BGFXSystem;

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ImageResource;
import Core.NativeWindowHandles;
import Core.RawDataResource;
import Core.ResourceHandle;
import Core.Spatial;
import Core.TypeId;
import Core.Window;
import Gfx.BGFXContext;
import Gfx.BGFXDrawCallback;
import Gfx.MaterialResource;
import Gfx.RenderObject;
import Gfx.ShaderDescriptor;
import Gfx.ShaderProgramDescriptor;
import Gfx.ShaderProgramResource;
import Gfx.TextureCoordinates;

namespace Gfx::BGFXSystemInternal {

	struct BGFXTexture {
		bgfx::TextureHandle textureHandle{};
	};

	struct BGFXShader {
		bgfx::ShaderHandle shaderHandle{};
	};

	struct BGFXProgram {
		bgfx::ProgramHandle programHandle{};
	};

	struct BGFXQuadMesh {
		bgfx::VertexBufferHandle vertexBuffer{};
		bgfx::IndexBufferHandle indexBuffer{};
		bgfx::UniformHandle textureUniformHandle{};
	};

	struct BGFXVertexLayout {
		bgfx::VertexLayout vertexLayout{};
	};

	struct BGFXUniforms {
		std::unordered_map<std::string, bgfx::UniformHandle> uniforms{};
	};

	struct QuadBufferData {
		bgfx::TransientVertexBuffer transientVertexBuffer{};
		bgfx::TransientIndexBuffer transientIndexBuffer{};
		uint32_t quadCount{};
		uint32_t maxVertexCount{};
		uint32_t maxIndexCount{};
	};

	std::optional<entt::entity> getMaterialResourceIfReadyToRender(const entt::registry& registry,
																   const entt::entity materialResourceHandle) {
		if (materialResourceHandle == entt::null || !registry.all_of<Core::ResourceHandle>(materialResourceHandle)) {
			return std::nullopt;
		}

		const auto& resourceHandle = registry.get<Core::ResourceHandle>(materialResourceHandle);

		const entt::entity materialResourceEntity = resourceHandle.mResourceEntity;
		if (materialResourceEntity == entt::null || !registry.all_of<MaterialResource>(materialResourceEntity)) {
			return std::nullopt;
		}

		const auto& materialResource = registry.get<MaterialResource>(materialResourceEntity);

		if (materialResource.textureImageResource == entt::null ||
			!registry.all_of<Core::ResourceHandle>(materialResource.textureImageResource)) {

			return std::nullopt;
		}

		if (materialResource.shaderProgramResource == entt::null ||
			!registry.all_of<Core::ResourceHandle>(materialResource.shaderProgramResource)) {

			return std::nullopt;
		}

		const auto& textureImageResourceHandle =
				registry.get<Core::ResourceHandle>(materialResource.textureImageResource);

		if (textureImageResourceHandle.mResourceEntity == entt::null ||
			!registry.all_of<BGFXTexture>(textureImageResourceHandle.mResourceEntity)) {

			return std::nullopt;
		}

		const auto& shaderProgramResourceHandle =
				registry.get<Core::ResourceHandle>(materialResource.shaderProgramResource);

		if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
			!registry.all_of<BGFXProgram>(shaderProgramResourceHandle.mResourceEntity)) {

			return std::nullopt;
		}

		return materialResourceEntity;
	}

	using PerMaterialBufferMap = std::unordered_map<entt::entity, QuadBufferData>;

	PerMaterialBufferMap createPerMaterialBuffers(entt::registry& registry) {
		PerMaterialBufferMap materialBuffers;

		registry.view<RenderObject>().each([&registry, &materialBuffers](const RenderObject& renderObject) {
			auto materialResourceEntity = getMaterialResourceIfReadyToRender(registry, renderObject.materialResource);
			if (!materialResourceEntity) {
				return;
			}

			const MaterialResource& materialResource = registry.get<MaterialResource>(*materialResourceEntity);
			if (renderObject.currentSpriteFrame >= materialResource.spriteFrames.size()) {
				return;
			}

			const auto& shaderProgramResourceHandle = registry.get<Core::ResourceHandle>(materialResource.shaderProgramResource);
			if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
				!registry.all_of<BGFXVertexLayout>(shaderProgramResourceHandle.mResourceEntity)) {
				return;
			}

			const auto& vertexLayout{ registry.get<BGFXVertexLayout>(shaderProgramResourceHandle.mResourceEntity) };

			constexpr uint32_t maxVertices = (32 << 10);
			constexpr uint32_t maxIndices = (32 << 10);
			if (!materialBuffers.contains(*materialResourceEntity)) {
				if (bgfx::getAvailTransientVertexBuffer(maxVertices, vertexLayout.vertexLayout) < maxVertices ||
					bgfx::getAvailTransientIndexBuffer(maxIndices) < maxIndices) {
					return;
				}

				bgfx::TransientVertexBuffer transientVertexBuffer{};
				bgfx::allocTransientVertexBuffer(&transientVertexBuffer, maxVertices, vertexLayout.vertexLayout);

				bgfx::TransientIndexBuffer transientIndexBuffer{};
				bgfx::allocTransientIndexBuffer(&transientIndexBuffer, maxIndices);

				materialBuffers.emplace(
						*materialResourceEntity,
						QuadBufferData{transientVertexBuffer, transientIndexBuffer, 0, maxVertices, maxIndices});
			}
		});

		return materialBuffers;
	}

	void extractRenderObjectToMaterialBuffer(entt::registry& registry, QuadBufferData& bufferData,
											 const Core::Spatial& spatial, const RenderObject& renderObject,
											 const MaterialResource& materialResource) {

		if (renderObject.currentSpriteFrame >= materialResource.spriteFrames.size()) {
			return;
		}

		const auto& shaderProgramResourceHandle = registry.get<Core::ResourceHandle>(materialResource.shaderProgramResource);
		if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
			!registry.all_of<BGFXVertexLayout>(shaderProgramResourceHandle.mResourceEntity)) {
			return;
			}

		const auto& vertexLayout{ registry.get<BGFXVertexLayout>(shaderProgramResourceHandle.mResourceEntity) };

		const auto& frameCoords{materialResource.spriteFrames[renderObject.currentSpriteFrame]};
		const uint32_t startVertex = bufferData.quadCount * 4;
		const uint32_t startIndex = bufferData.quadCount * 6;
		if (startVertex + 4 >= bufferData.maxVertexCount || startIndex + 6 >= bufferData.maxIndexCount) {
			return;
		}

		const float quadWidth = (frameCoords.x1 - frameCoords.x0) * 2.0f; // / materialResource.width;
		const float quadHeight = (frameCoords.y1 - frameCoords.y0) * 2.0f; // / materialResource.height;

		const TextureCoordinates factoredCoords{
				frameCoords.x0 / materialResource.width, frameCoords.y0 / materialResource.height,
				frameCoords.x1 / materialResource.width, frameCoords.y1 / materialResource.height};

		using DecodedAttributes = std::tuple<uint8_t, bgfx::AttribType::Enum, bool, bool>;
		DecodedAttributes posAttributes;
		vertexLayout.vertexLayout.decode(bgfx::Attrib::Position,
			std::get<0>(posAttributes),
			std::get<1>(posAttributes),
			std::get<2>(posAttributes),
			std::get<3>(posAttributes)
		);

		DecodedAttributes texCoord0Attributes;
		vertexLayout.vertexLayout.decode(bgfx::Attrib::TexCoord0,
			std::get<0>(texCoord0Attributes),
			std::get<1>(texCoord0Attributes),
			std::get<2>(texCoord0Attributes),
			std::get<3>(texCoord0Attributes)
		);

		auto&& [posCount, posType, _posNormalized, _posAsInt] = posAttributes;
		auto&& [texCoord0Count, texCoord0Type, _texCoord0Normalized, _texCoord0AsInt] = texCoord0Attributes;

		if (posCount != 3 || posType != bgfx::AttribType::Float) {
			return;
		}

		if (texCoord0Count != 2 || texCoord0Type != bgfx::AttribType::Float) {
			return;
		}

		for (size_t i = 0; i < 4; ++i) {
			uint8_t* vertexHead = bufferData.transientVertexBuffer.data + vertexLayout.vertexLayout.getStride() * (startVertex + i);
			float* positionHead = reinterpret_cast<float*>(vertexHead + vertexLayout.vertexLayout.getOffset(bgfx::Attrib::Position));
			float* texCoordHead = reinterpret_cast<float*>(vertexHead + vertexLayout.vertexLayout.getOffset(bgfx::Attrib::TexCoord0));

			if (i == 0) {
				positionHead[0] = spatial.x;
				positionHead[1] = spatial.y;
				positionHead[2] = spatial.z;
				texCoordHead[0] = factoredCoords.x0;
				texCoordHead[1] = factoredCoords.y0;
			} else if (i == 1) {
				positionHead[0] = spatial.x;
				positionHead[1] = spatial.y + quadHeight;
				positionHead[2] = spatial.z;
				texCoordHead[0] = factoredCoords.x0;
				texCoordHead[1] = factoredCoords.y1;
			} else if (i == 2) {
				positionHead[0] = spatial.x + quadWidth;
				positionHead[1] = spatial.y + quadHeight;
				positionHead[2] = spatial.z;
				texCoordHead[0] = factoredCoords.x1;
				texCoordHead[1] = factoredCoords.y1;
			} else if (i == 3) {
				positionHead[0] = spatial.x + quadWidth;
				positionHead[1] = spatial.y;
				positionHead[2] = spatial.z;
				texCoordHead[0] = factoredCoords.x1;
				texCoordHead[1] = factoredCoords.y0;
			} else {
				assert(false);
			}
		}

		uint16_t* indexHead = reinterpret_cast<uint16_t*>(bufferData.transientIndexBuffer.data) + startIndex;

		// 0, 1, 3,
		indexHead[0] = startVertex;
		indexHead[1] = startVertex + 1;
		indexHead[2] = startVertex + 3;
		// 3, 1, 2
		indexHead[3] = startVertex + 3;
		indexHead[4] = startVertex + 1;
		indexHead[5] = startVertex + 2;

		++bufferData.quadCount;
	}

	void extractRenderObjectsToPerMaterialBuffers(entt::registry& registry, PerMaterialBufferMap& materialBuffers) {
		registry.view<Core::Spatial, RenderObject>().each([&registry, &materialBuffers](auto& spatial,
																						auto& renderObject) {
			auto materialResourceEntity = getMaterialResourceIfReadyToRender(registry, renderObject.materialResource);
			if (!materialResourceEntity) {
				return;
			}

			if (!materialBuffers.contains(*materialResourceEntity)) {
				return;
			}

			auto& quadBufferData{materialBuffers[*materialResourceEntity]};
			const MaterialResource& materialResource = registry.get<MaterialResource>(*materialResourceEntity);
			extractRenderObjectToMaterialBuffer(registry, quadBufferData, spatial, renderObject, materialResource);
		});
	}

	void renderPerMaterialBuffers(entt::registry& registry, const PerMaterialBufferMap& materialBuffers) {

		for (const auto& [materialEntity, transientBufferData] : materialBuffers) {
			if (materialEntity == entt::null || !registry.all_of<MaterialResource>(materialEntity)) {
				continue;
			}

			const MaterialResource& materialResource = registry.get<MaterialResource>(materialEntity);
			if (!registry.all_of<Core::ResourceHandle>(materialResource.textureImageResource) ||
				!registry.all_of<Core::ResourceHandle>(materialResource.shaderProgramResource)) {
				return;
			}

			const auto& textureImageResourceHandle =
					registry.get<Core::ResourceHandle>(materialResource.textureImageResource);
			if (textureImageResourceHandle.mResourceEntity == entt::null ||
				!registry.all_of<BGFXTexture>(textureImageResourceHandle.mResourceEntity)) {
				return;
			}

			const auto& shaderProgramResourceHandle =
					registry.get<Core::ResourceHandle>(materialResource.shaderProgramResource);
			if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
				!registry.all_of<BGFXProgram>(shaderProgramResourceHandle.mResourceEntity) ||
				!registry.all_of<BGFXUniforms>(shaderProgramResourceHandle.mResourceEntity)) {
				return;
			}

			const auto& bgfxProgram = registry.get<BGFXProgram>(shaderProgramResourceHandle.mResourceEntity);
			const auto& bgfxUniforms = registry.get<BGFXUniforms>(shaderProgramResourceHandle.mResourceEntity);
			const auto& bgfxTexture = registry.get<BGFXTexture>(textureImageResourceHandle.mResourceEntity);

			if (!bgfxUniforms.uniforms.contains("s_texColour") || !bgfxUniforms.uniforms.contains("u_alphaColour")) {
				return;
			}

			bgfx::UniformHandle textureColourUniform{ bgfxUniforms.uniforms.at("s_texColour") };
			bgfx::UniformHandle alphaColourUniform{ bgfxUniforms.uniforms.at("u_alphaColour") };

			float mtx[16];
			bx::mtxIdentity(mtx);
			bgfx::setTransform(mtx);

			bgfx::setVertexBuffer(0, &transientBufferData.transientVertexBuffer, 0, transientBufferData.quadCount * 4);
			bgfx::setIndexBuffer(&transientBufferData.transientIndexBuffer, 0, transientBufferData.quadCount * 6);
			bgfx::setTexture(0, textureColourUniform, bgfxTexture.textureHandle);

			const auto& alphaColour = materialResource.alphaColour.value_or({255.0f, 255.0f, 255.0f, 255.0f});
			bgfx::setUniform(alphaColourUniform, &alphaColour);

			// Set render states.
			bgfx::setState(0 | BGFX_STATE_WRITE_RGB |
						   BGFX_STATE_WRITE_A
						   //| BGFX_STATE_WRITE_Z
						   //| BGFX_STATE_DEPTH_TEST_LESS
						   | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA) |
						   BGFX_STATE_BLEND_EQUATION_ADD
						   //| BGFX_STATE_MSAA
			);

			// Submit primitive for rendering to view 0.
			bgfx::submit(0, bgfxProgram.programHandle);
		}
	}

} // namespace Gfx::BGFXSystemInternal

namespace Gfx {

	void BGFXSystem::onWindowInternalCreated(entt::registry& registry, entt::entity entity) {
		if (!registry.view<const BGFXContext>().empty()) {
			return;
		}

		auto&& [nativeWindowHandles, window] = registry.get<Core::NativeWindowHandles, Core::Window>(entity);

		if (!nativeWindowHandles.windowHandle) {
			return;
		}

		bgfx::Init initData;
		initData.platformData.nwh = nativeWindowHandles.windowHandle;
		initData.platformData.ndt = nativeWindowHandles.displayHandle;
		initData.resolution.width = window.width;
		initData.resolution.height = window.height;

		if (!bgfx::init(initData)) {
			return;
		}

		entt::entity contextEntity = registry.create();
		registry.emplace<BGFXContext>(contextEntity);
	}

	BGFXSystem::BGFXSystem(Core::EnTTRegistry& registry) : mRegistry{registry} {}

	BGFXSystem::~BGFXSystem() = default;

	void BGFXSystem::initSystem(entt::registry& registry) {
		registry.on_construct<Core::NativeWindowHandles>().connect<&BGFXSystem::onWindowInternalCreated>(this);
	}

	void BGFXSystem::destroySystem(entt::registry& registry) {
		if (registry.view<const BGFXContext>().empty()) {
			return;
		}

		registry.view<BGFXContext>().each([this](BGFXContext& context) { _destroyTrackedTextures(context); });

		bgfx::shutdown();
	}

	void BGFXSystem::tickSystem(entt::registry& registry) {
		using namespace BGFXSystemInternal;

		auto bgfxContextView = registry.view<BGFXContext>();
		if (bgfxContextView.empty()) {
			return;
		}

		auto bgfxContextEntity{bgfxContextView.front()};
		auto& bgfxContext{registry.get<BGFXContext>(bgfxContextEntity)};

		registry.view<const ShaderProgramResource>(entt::exclude<BGFXUniforms>)
				.each([&registry, &bgfxContext](entt::entity entity,
													const ShaderProgramResource& shaderProgramResource) {
					_tryCreateUniforms(bgfxContext, registry, entity, shaderProgramResource);
				});

		registry.view<const ShaderDescriptor, const Core::RawDataResource>(
						entt::exclude<Core::FileLoadRequest, BGFXShader>)
				.each([&registry, &bgfxContext](entt::entity entity, const Core::RawDataResource& rawDataResource) {
					_tryCreateShader(bgfxContext, registry, entity, rawDataResource);
				});

		registry.view<const ShaderProgramResource>(entt::exclude<BGFXProgram>)
				.each([&registry, &bgfxContext](entt::entity entity,
												const ShaderProgramResource& shaderProgramResource) {
					_tryCreateShaderProgram(bgfxContext, registry, entity, shaderProgramResource);
				});

		registry.view<const ShaderProgramResource>(entt::exclude<BGFXVertexLayout>)
				.each([&registry, &bgfxContext](entt::entity entity,
												const ShaderProgramResource& shaderProgramResource) {
					_tryCreateVertexLayout(bgfxContext, registry, entity, shaderProgramResource);
				});

		registry.view<const Core::ImageResource>(entt::exclude<BGFXTexture>)
				.each([&registry, &bgfxContext](entt::entity entity, const Core::ImageResource& imageResource) {
					_tryCreateTexture(bgfxContext, registry, entity, imageResource);
				});

		const bgfx::ViewId viewId{0};
		bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
		bgfx::setViewRect(viewId, 0, 0, bgfx::BackbufferRatio::Equal);

		bgfx::touch(viewId);
		bgfx::dbgTextClear();
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		float view[16];
		bx::mtxIdentity(view);

		float proj[16];
		bx::mtxOrtho(proj, 0.0f, 1360.0f, 768.0f, 0.0f, 0.0f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		// TODO: pre-sort render objects by material and THEN create and submit buffers one by one.
		using namespace BGFXSystemInternal;
		auto materialBuffers = createPerMaterialBuffers(registry);
		extractRenderObjectsToPerMaterialBuffers(registry, materialBuffers);
		renderPerMaterialBuffers(registry, materialBuffers);

		registry.view<BGFXDrawCallback>().each(
				[&registry](const BGFXDrawCallback& drawCallback) { drawCallback.drawCallback(registry); });

		bgfx::frame();

		registry.view<BGFXContext>().each(
				[this, &registry](BGFXContext& context) { _destroyDroppedTextures(registry, context); });
	}

	void BGFXSystem::_destroyDroppedTextures(const entt::registry& registry, BGFXContext& context) {
		using namespace BGFXSystemInternal;
		for (auto it = context.trackedTextureHandles.begin(); it != context.trackedTextureHandles.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->first) && registry.all_of<BGFXTexture>(currentIt->first)) {
				continue;
			}

			bgfx::destroy(currentIt->second);

			context.trackedTextureHandles.erase(currentIt);
		}

		for (auto it = context.trackedShaderHandles.begin(); it != context.trackedShaderHandles.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->first) && registry.all_of<BGFXShader>(currentIt->first)) {
				continue;
			}

			bgfx::destroy(currentIt->second);

			context.trackedShaderHandles.erase(currentIt);
		}

		for (auto it = context.trackedShaderProgramHandles.begin(); it != context.trackedShaderProgramHandles.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->first) && registry.all_of<BGFXProgram>(currentIt->first)) {
				continue;
			}

			bgfx::destroy(currentIt->second);

			context.trackedShaderProgramHandles.erase(currentIt);
		}
	}

	void BGFXSystem::_destroyTrackedTextures(BGFXContext& context) const {
		for (const auto textureHandle : context.trackedTextureHandles | std::views::values) {
			bgfx::destroy(textureHandle);
		}

		for (const auto shaderHandle : context.trackedShaderHandles | std::views::values) {
			bgfx::destroy(shaderHandle);
		}

		for (const auto shaderProgramHandle : context.trackedShaderProgramHandles | std::views::values) {
			bgfx::destroy(shaderProgramHandle);
		}

		context.trackedTextureHandles.clear();
		context.trackedShaderHandles.clear();
		context.trackedShaderProgramHandles.clear();
	}

	void BGFXSystem::_tryCreateShader(BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity,
									  const Core::RawDataResource& rawDataResource) {
		using namespace BGFXSystemInternal;

		const bgfx::Memory* mem = bgfx::copy(rawDataResource.rawData.data(), rawDataResource.size);
		const bgfx::ShaderHandle shaderHandle = bgfx::createShader(mem);
		if (shaderHandle.idx == bgfx::kInvalidHandle) {
			return;
		}

		registry.emplace<BGFXShader>(entity, shaderHandle);
		bgfxContext.trackedShaderHandles.emplace_back(entity, shaderHandle);
	}

	void BGFXSystem::_tryCreateShaderProgram(BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity,
											 const ShaderProgramResource& shaderProgramResource) {
		using namespace BGFXSystemInternal;

		if (shaderProgramResource.vertexShaderResource == entt::null ||
			shaderProgramResource.fragmentShaderResource == entt::null) {
			return;
		}

		if (!registry.all_of<Core::ResourceHandle>(shaderProgramResource.vertexShaderResource) ||
			!registry.all_of<Core::ResourceHandle>(shaderProgramResource.fragmentShaderResource)) {
			return;
		}

		const auto& vsResourceHandle{registry.get<Core::ResourceHandle>(shaderProgramResource.vertexShaderResource)};
		const auto& fsResourceHandle{registry.get<Core::ResourceHandle>(shaderProgramResource.fragmentShaderResource)};

		if (!registry.all_of<BGFXShader>(vsResourceHandle.mResourceEntity) ||
			!registry.all_of<BGFXShader>(fsResourceHandle.mResourceEntity)) {
			return;
		}

		const auto& vertexShader = registry.get<BGFXShader>(vsResourceHandle.mResourceEntity);
		const auto& fragmentShader = registry.get<BGFXShader>(fsResourceHandle.mResourceEntity);

		if (vertexShader.shaderHandle.idx == bgfx::kInvalidHandle ||
			fragmentShader.shaderHandle.idx == bgfx::kInvalidHandle) {
			return;
		}

		bgfx::ProgramHandle program = bgfx::createProgram(vertexShader.shaderHandle, fragmentShader.shaderHandle);
		if (program.idx == bgfx::kInvalidHandle) {
			return;
		}

		registry.emplace<BGFXProgram>(entity, program);
		bgfxContext.trackedShaderProgramHandles.emplace_back(entity, program);
	}

	void BGFXSystem::_tryCreateVertexLayout(BGFXContext&,
		entt::registry& registry, entt::entity entity, const ShaderProgramResource& shaderProgramResource) {

		using namespace BGFXSystemInternal;
		bgfx::VertexLayout vertexLayout{};
		vertexLayout.begin();

		for (auto&& inputAttribute : shaderProgramResource.vertexLayout.attributes) {
			bgfx::Attrib::Enum attribute{};
			switch (inputAttribute.attributeId) {
				case ShaderVertexLayoutAttributeId::Position:
					attribute = bgfx::Attrib::Position;
					break;
				case ShaderVertexLayoutAttributeId::TexCoord0:
					attribute = bgfx::Attrib::TexCoord0;
					break;
				default:
					vertexLayout.end();
					return;
			}

			bgfx::AttribType::Enum attributeType{};
			if (inputAttribute.typeId == Core::TypeId::get<float>()) {
				attributeType = bgfx::AttribType::Float;
			} else {
				printf("Unhandled attribute type.\n");
				vertexLayout.end();
				return;
			}

			vertexLayout.add(attribute, inputAttribute.count, attributeType);
		}

		vertexLayout.end();
		registry.emplace<BGFXVertexLayout>(entity, vertexLayout);
	}

	void BGFXSystem::_tryCreateUniforms(BGFXContext&,
		entt::registry& registry, entt::entity entity, const ShaderProgramResource& shaderProgramResource) {

		using namespace BGFXSystemInternal;
		BGFXUniforms bgfxUniforms{};
		for (auto&& uniform : shaderProgramResource.uniforms) {
			if (bgfxUniforms.uniforms.contains(uniform.name)) {
				printf("Uniform with duplicate name encountered.\n");
				return;
			}

			bgfx::UniformType::Enum uniformType{};
			switch (uniform.type) {
				case ShaderUniformType::Sampler:
					uniformType = bgfx::UniformType::Sampler;
					break;
				case ShaderUniformType::Float4:
					uniformType = bgfx::UniformType::Vec4;
					break;
				default:
					printf("Unhandled uniform type.\n");
					return;
			}

			bgfx::UniformHandle uniformHandle{ bgfx::createUniform(uniform.name.c_str(), uniformType) };
			bgfxUniforms.uniforms.emplace(uniform.name, uniformHandle);
		}

		registry.emplace<BGFXUniforms>(entity, std::move(bgfxUniforms));
	}

	void BGFXSystem::_tryCreateTexture(BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity,
									   const Core::ImageResource& imageResource) {
		using namespace BGFXSystemInternal;

		const bgfx::Memory* mem = bgfx::copy(imageResource.image.data(), imageResource.image.size());
		bgfx::TextureHandle texture = bgfx::createTexture2D(
				imageResource.width, imageResource.height, imageResource.numMips > 1, imageResource.numLayers,
				static_cast<bgfx::TextureFormat::Enum>(imageResource.format), BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
				mem);

		registry.emplace<BGFXTexture>(entity, texture);
		bgfxContext.trackedTextureHandles.emplace_back(entity, texture);
	}

} // namespace Gfx
