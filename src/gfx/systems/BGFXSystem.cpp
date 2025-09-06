module;

#include <cstring>

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <entt/entt.hpp>

module Gfx.BGFXSystem;

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.NativeWindowHandles;
import Core.RawDataResource;
import Core.ResourceHandle;
import Core.Spatial;
import Core.TypeId;
import Core.Window;
import Gfx.BGFXContext;
import Gfx.BGFXDrawCallback;
import Gfx.Camera;
import Gfx.Colour;
import Gfx.Image;
import Gfx.IndexBuffer;
import Gfx.Material;
import Gfx.RenderCommand;
import Gfx.RenderObject;
import Gfx.ShaderDescriptor;
import Gfx.ShaderProgram;
import Gfx.ShaderProgramDescriptor;
import Gfx.TextureCoordinates;
import Gfx.VertexBuffer;
import Gfx.Viewport;

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

	struct BGFXVertexLayout {
		bgfx::VertexLayout vertexLayout{};
	};

	struct BGFXUniforms {
		std::unordered_map<std::string, bgfx::UniformHandle> uniforms{};
	};

	void processRenderCommand(entt::registry& registry, const RenderCommand& renderCommand) {
		if (!registry.all_of<Viewport>(renderCommand.viewportEntity)) {
			return;
		}

		const Viewport& viewport{ registry.get<Viewport>(renderCommand.viewportEntity) };
		if (!registry.all_of<Camera, Core::Spatial>(viewport.camera)) {
			return;
		}

		// const auto& camera{ registry.get<Camera>(viewport.camera) };
		const auto& spatial{ registry.get<Core::Spatial>(viewport.camera) };

		constexpr float width = 1360.0f;
		constexpr float height = 768.0f;

		const float left = viewport.x * width;
		const float right = viewport.width * width;
		const float top = viewport.y * height;
		const float bottom = viewport.height * height;

		bgfx::setViewRect(renderCommand.renderPass, left, top, right - left, bottom - top);

		float view[16];
		bx::mtxIdentity(view);
		bx::mtxTranslate(view, spatial.x, spatial.y, spatial.z);

		float proj[16];
		bx::mtxOrtho(proj, left, right, bottom, top, 0.0f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(renderCommand.renderPass, view, proj);

		if (!registry.all_of<VertexBuffer>(renderCommand.vertexBuffer) ||
			!registry.all_of<IndexBuffer>(renderCommand.indexBuffer) ||
			!registry.all_of<ShaderProgram>(renderCommand.shaderProgram) ||
			!registry.all_of<Image>(renderCommand.texture)) {

			return;
		}

		auto& vertexBuffer{ registry.get<VertexBuffer>(renderCommand.vertexBuffer) };
		auto& indexBuffer{ registry.get<IndexBuffer>(renderCommand.indexBuffer) };

		if (vertexBuffer.type != VertexBufferType::Transient ||
			indexBuffer.type != IndexBufferType::Transient) {
			return;
		}

		if (!registry.all_of<BGFXProgram, BGFXUniforms, BGFXVertexLayout>(renderCommand.shaderProgram)) {
			return;
		}

		if (!registry.all_of<BGFXTexture>(renderCommand.texture)) {
			return;
		}

		const auto& vertexLayout{ registry.get<BGFXVertexLayout>(renderCommand.shaderProgram) };

		constexpr uint32_t maxVertices = (32 << 10);
		constexpr uint32_t maxIndices = (32 << 10);

		if (bgfx::getAvailTransientVertexBuffer(maxVertices, vertexLayout.vertexLayout) < maxVertices ||
			bgfx::getAvailTransientIndexBuffer(maxIndices) < maxIndices) {
			return;
		}

		bgfx::TransientVertexBuffer transientVertexBuffer{};
		bgfx::allocTransientVertexBuffer(&transientVertexBuffer, maxVertices, vertexLayout.vertexLayout);

		std::memcpy(transientVertexBuffer.data, vertexBuffer.data.data(), vertexBuffer.data.size());

		bgfx::TransientIndexBuffer transientIndexBuffer{};
		bgfx::allocTransientIndexBuffer(&transientIndexBuffer, maxIndices, indexBuffer.is32Bit);

		std::memcpy(transientIndexBuffer.data, indexBuffer.data.data(), indexBuffer.data.size());

		const auto& bgfxProgram = registry.get<BGFXProgram>(renderCommand.shaderProgram);
		const auto& bgfxUniforms = registry.get<BGFXUniforms>(renderCommand.shaderProgram);
		const auto& bgfxTexture = registry.get<BGFXTexture>(renderCommand.texture);

		if (!bgfxUniforms.uniforms.contains("s_texColour") || !bgfxUniforms.uniforms.contains("u_alphaColour")) {
			return;
		}

		bgfx::UniformHandle textureColourUniform{ bgfxUniforms.uniforms.at("s_texColour") };
		bgfx::UniformHandle alphaColourUniform{ bgfxUniforms.uniforms.at("u_alphaColour") };

		float mtx[16];
		bx::mtxIdentity(mtx);
		bgfx::setTransform(mtx);

		bgfx::setVertexBuffer(0, &transientVertexBuffer, 0, renderCommand.vertexCount);
		bgfx::setIndexBuffer(&transientIndexBuffer, 0, renderCommand.indexCount);
		bgfx::setTexture(0, textureColourUniform, bgfxTexture.textureHandle);

		const auto alphaColour = Colour{ 0.0f, 0.0f, 0.0f, 0.0f };
		bgfx::setUniform(alphaColourUniform, &alphaColour);

		// Set render states.
		bgfx::setState(
			0 | BGFX_STATE_WRITE_RGB |
			BGFX_STATE_WRITE_A
			//| BGFX_STATE_WRITE_Z
			//| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA) |
			BGFX_STATE_BLEND_EQUATION_ADD
			//| BGFX_STATE_MSAA
		);

		// Submit primitive for rendering to view 0.
		bgfx::submit(renderCommand.renderPass, bgfxProgram.programHandle);
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

	BGFXSystem::BGFXSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		initSystem(mRegistry);

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	BGFXSystem::~BGFXSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
		destroySystem(mRegistry);
	}

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

		auto bgfxContextEntity{ bgfxContextView.front() };
		auto& bgfxContext{ registry.get<BGFXContext>(bgfxContextEntity) };

		registry.view<const ShaderProgram>(entt::exclude<BGFXUniforms>)
			.each([&registry, &bgfxContext](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateUniforms(bgfxContext, registry, entity, shaderProgram);
			});

		registry
			.view<const ShaderDescriptor, const Core::RawDataResource>(entt::exclude<Core::FileLoadRequest, BGFXShader>)
			.each([&registry, &bgfxContext](entt::entity entity, const Core::RawDataResource& rawDataResource) {
				_tryCreateShader(bgfxContext, registry, entity, rawDataResource);
			});

		registry.view<const ShaderProgram>(entt::exclude<BGFXProgram>)
			.each([&registry, &bgfxContext](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateShaderProgram(bgfxContext, registry, entity, shaderProgram);
			});

		registry.view<const ShaderProgram>(entt::exclude<BGFXVertexLayout>)
			.each([&registry, &bgfxContext](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateVertexLayout(bgfxContext, registry, entity, shaderProgram);
			});

		registry.view<const Image>(entt::exclude<BGFXTexture>)
			.each([&registry, &bgfxContext](entt::entity entity, const Image& image) {
				_tryCreateTexture(bgfxContext, registry, entity, image);
			});

		bgfx::ViewId currentViewId{ 0 };
		registry.view<Viewport>().each([&currentViewId, &registry](const Viewport& viewport) {
			const bgfx::ViewId viewId = currentViewId++;
			constexpr float width = 1360.0f;
			constexpr float height = 768.0f;

			const float left = viewport.x * width;
			const float right = viewport.width * width;
			const float top = viewport.y * height;
			const float bottom = viewport.height * height;

			bgfx::setViewRect(0, left, top, right - left, bottom - top);
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);

			bgfx::touch(viewId);
			bgfx::dbgTextClear();
			bgfx::setDebug(BGFX_DEBUG_TEXT);
		});

		std::unordered_map<bgfx::ViewId, std::vector<RenderCommand>> commandBuckets;
		registry.view<RenderCommand>().each([&commandBuckets, &registry](const RenderCommand& renderCommand) {
			commandBuckets[renderCommand.renderPass].emplace_back(renderCommand);
		});

		for (auto&& [viewId, renderCommands] : commandBuckets) {
			for (auto&& renderCommand : renderCommands) {
				processRenderCommand(registry, renderCommand);
			}
		}

		registry.clear<RenderCommand>();
		registry.clear<VertexBuffer>();
		registry.clear<IndexBuffer>();

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

	void BGFXSystem::_tryCreateShader(
		BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity,
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

	void BGFXSystem::_tryCreateShaderProgram(
		BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {
		using namespace BGFXSystemInternal;

		if (shaderProgram.vertexShader == entt::null || shaderProgram.fragmentShader == entt::null) {
			return;
		}

		if (!registry.all_of<Core::ResourceHandle>(shaderProgram.vertexShader) ||
			!registry.all_of<Core::ResourceHandle>(shaderProgram.fragmentShader)) {
			return;
		}

		const auto& vsResourceHandle{ registry.get<Core::ResourceHandle>(shaderProgram.vertexShader) };
		const auto& fsResourceHandle{ registry.get<Core::ResourceHandle>(shaderProgram.fragmentShader) };

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

	void BGFXSystem::_tryCreateVertexLayout(
		BGFXContext&, entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {

		using namespace BGFXSystemInternal;
		bgfx::VertexLayout vertexLayout{};
		vertexLayout.begin();

		for (auto&& inputAttribute : shaderProgram.vertexLayout.attributes) {
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

	void BGFXSystem::_tryCreateUniforms(
		BGFXContext&, entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {

		using namespace BGFXSystemInternal;
		BGFXUniforms bgfxUniforms{};
		for (auto&& uniform : shaderProgram.uniforms) {
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

	void BGFXSystem::_tryCreateTexture(
		BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity, const Image& imageResource) {
		using namespace BGFXSystemInternal;

		const bgfx::Memory* mem = bgfx::copy(imageResource.image.data(), imageResource.image.size());
		bgfx::TextureHandle texture = bgfx::createTexture2D(
			imageResource.width, imageResource.height, imageResource.numMips > 1, imageResource.numLayers,
			static_cast<bgfx::TextureFormat::Enum>(imageResource.format), BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, mem);

		registry.emplace<BGFXTexture>(entity, texture);
		bgfxContext.trackedTextureHandles.emplace_back(entity, texture);
	}

} // namespace Gfx
