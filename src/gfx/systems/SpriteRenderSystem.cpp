module;

#include <entt/entt.hpp>

module Gfx.SpriteRenderSystem;

import Core.ResourceHandle;
import Core.Spatial;
import Gfx.Image;
import Gfx.IndexBuffer;
import Gfx.Material;
import Gfx.RenderCommand;
import Gfx.RenderObject;
import Gfx.ShaderProgram;
import Gfx.ShaderProgramDescriptor;
import Gfx.Sprite;
import Gfx.TextureCoordinates;
import Gfx.VertexBuffer;
import Gfx.Viewport;

namespace Gfx::SpriteRenderSystemInternal {

	struct MaterialBuffers {
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;
		size_t maxVertexCount{};
		size_t maxIndexCount{};
		size_t quadCount{};
	};

	using PerMaterialBuffersMap = std::unordered_map<entt::entity, MaterialBuffers>;

	std::optional<entt::entity>
		getMaterialResourceIfReadyToRender(const entt::registry& registry, const entt::entity materialResourceHandle) {
		if (materialResourceHandle == entt::null || !registry.all_of<Core::ResourceHandle>(materialResourceHandle)) {
			return std::nullopt;
		}

		const auto& resourceHandle = registry.get<Core::ResourceHandle>(materialResourceHandle);

		const entt::entity materialResourceEntity = resourceHandle.mResourceEntity;
		if (materialResourceEntity == entt::null || !registry.all_of<Material>(materialResourceEntity)) {
			return std::nullopt;
		}

		const auto& materialResource = registry.get<Material>(materialResourceEntity);

		if (materialResource.textureImage == entt::null ||
			!registry.all_of<Core::ResourceHandle>(materialResource.textureImage)) {

			return std::nullopt;
			}

		if (materialResource.shaderProgram == entt::null ||
			!registry.all_of<Core::ResourceHandle>(materialResource.shaderProgram)) {

			return std::nullopt;
		}

		// const auto& textureImageResourceHandle = registry.get<Core::ResourceHandle>(materialResource.textureImage);
		//
		// if (textureImageResourceHandle.mResourceEntity == entt::null ||
		// 	!registry.all_of<BGFXTexture>(textureImageResourceHandle.mResourceEntity)) {
		//
		// 	return std::nullopt;
		// 	}
		//
		// const auto& shaderProgramResourceHandle = registry.get<Core::ResourceHandle>(materialResource.shaderProgram);
		//
		// if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
		// 	!registry.all_of<BGFXProgram>(shaderProgramResourceHandle.mResourceEntity)) {
		//
		// 	return std::nullopt;
		// 	}

		return materialResourceEntity;
	}

	PerMaterialBuffersMap createPerMaterialBuffers(entt::registry& registry) {
		PerMaterialBuffersMap perMaterialBuffers;

		registry.view<RenderObject, Sprite>().each([&registry, &perMaterialBuffers](const RenderObject& renderObject, const Sprite&) {
			auto materialResourceEntity = getMaterialResourceIfReadyToRender(registry, renderObject.materialResource);
			if (!materialResourceEntity) {
				return;
			}

			const Material& materialResource = registry.get<Material>(*materialResourceEntity);
			if (renderObject.currentSpriteFrame >= materialResource.spriteFrames.size()) {
				return;
			}

			const auto& shaderProgramResourceHandle =
				registry.get<Core::ResourceHandle>(materialResource.shaderProgram);
			if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
				!registry.all_of<ShaderProgram>(shaderProgramResourceHandle.mResourceEntity)) {
				return;
			}

			const auto& shaderProgram{ registry.get<ShaderProgram>(shaderProgramResourceHandle.mResourceEntity) };
			const auto& vertexLayout{ shaderProgram.vertexLayout };

			constexpr uint32_t maxVertices = (32 << 10);
			constexpr uint32_t maxIndices = (32 << 10);
			if (!perMaterialBuffers.contains(*materialResourceEntity)) {
				VertexBuffer vertexBuffer;
				vertexBuffer.type = VertexBufferType::Transient;
				vertexBuffer.data.resize(getBufferSizeForVertexLayout(shaderProgram.vertexLayout, maxVertices));

				IndexBuffer indexBuffer;
				indexBuffer.is32Bit = false;

				const size_t indexElementSize = indexBuffer.is32Bit ? sizeof(uint32_t) : sizeof(uint16_t);
				indexBuffer.data.resize(indexElementSize * maxIndices);

				MaterialBuffers materialBuffers{
					std::move(vertexBuffer),
					std::move(indexBuffer),
					maxVertices,
					maxIndices
				};

				perMaterialBuffers.emplace(*materialResourceEntity, std::move(materialBuffers));
			}
		});

		return perMaterialBuffers;
	}

	void extractRenderObjectToMaterialBuffer(
		entt::registry& registry, MaterialBuffers& materialBuffers, const Core::Spatial& spatial,
		const RenderObject& renderObject, const Material& materialResource) {

		if (renderObject.currentSpriteFrame >= materialResource.spriteFrames.size()) {
			return;
		}

		const auto& shaderProgramResourceHandle = registry.get<Core::ResourceHandle>(materialResource.shaderProgram);
		if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
			!registry.all_of<ShaderProgram>(shaderProgramResourceHandle.mResourceEntity)) {
			return;
		}

		const auto& shaderProgram{ registry.get<ShaderProgram>(shaderProgramResourceHandle.mResourceEntity) };
		const auto& vertexLayout{ shaderProgram.vertexLayout };

		const auto& [x0, y0, x1, y1]{ materialResource.spriteFrames[renderObject.currentSpriteFrame] };
		const uint32_t startVertex = materialBuffers.quadCount * 4;
		const uint32_t startIndex = materialBuffers.quadCount * 6;
		if (startVertex + 4 >= materialBuffers.maxVertexCount || startIndex + 6 >= materialBuffers.maxIndexCount) {
			return;
		}

		const float halfQuadWidth = (x1 - x0) * spatial.scaleX * 0.5f;
		const float halfQuadHeight = (y1 - y0) * spatial.scaleY * 0.5f;

		const TextureCoordinates factoredCoords{ x0 / materialResource.width,
												 y0 / materialResource.height,
												 x1 / materialResource.width,
												 y1 / materialResource.height };

		for (size_t i = 0; i < 4; ++i) {
			uint8_t* vertexHead =
				materialBuffers.vertexBuffer.data.data() + getStrideForVertexLayout(vertexLayout) * (startVertex + i);
			auto* positionHead =
				reinterpret_cast<float*>(vertexHead + getOffsetForVertexLayoutAttribute(vertexLayout, ShaderVertexLayoutAttributeId::Position));
			auto* texCoordHead =
				reinterpret_cast<float*>(vertexHead + getOffsetForVertexLayoutAttribute(vertexLayout, ShaderVertexLayoutAttributeId::TexCoord0));

			if (i == 0) {
				positionHead[0] = spatial.x - halfQuadWidth;
				positionHead[1] = spatial.y - halfQuadHeight;
				positionHead[2] = spatial.z;
				texCoordHead[0] = factoredCoords.x0;
				texCoordHead[1] = factoredCoords.y0;
			} else if (i == 1) {
				positionHead[0] = spatial.x - halfQuadWidth;
				positionHead[1] = spatial.y + halfQuadHeight;
				positionHead[2] = spatial.z;
				texCoordHead[0] = factoredCoords.x0;
				texCoordHead[1] = factoredCoords.y1;
			} else if (i == 2) {
				positionHead[0] = spatial.x + halfQuadWidth;
				positionHead[1] = spatial.y + halfQuadHeight;
				positionHead[2] = spatial.z;
				texCoordHead[0] = factoredCoords.x1;
				texCoordHead[1] = factoredCoords.y1;
			} else if (i == 3) {
				positionHead[0] = spatial.x + halfQuadWidth;
				positionHead[1] = spatial.y - halfQuadHeight;
				positionHead[2] = spatial.z;
				texCoordHead[0] = factoredCoords.x1;
				texCoordHead[1] = factoredCoords.y0;
			} else {
				assert(false);
			}
		}

		uint16_t* indexHead = reinterpret_cast<uint16_t*>(materialBuffers.indexBuffer.data.data()) + startIndex;

		// 0, 1, 3,
		indexHead[0] = startVertex;
		indexHead[1] = startVertex + 1;
		indexHead[2] = startVertex + 3;
		// 3, 1, 2
		indexHead[3] = startVertex + 3;
		indexHead[4] = startVertex + 1;
		indexHead[5] = startVertex + 2;

		++materialBuffers.quadCount;
	}

	void extractRenderObjectsToPerMaterialBuffers(entt::registry& registry, PerMaterialBuffersMap& materialBuffers) {
		registry.view<Core::Spatial, RenderObject, Sprite>().each([&registry,
														   &materialBuffers](auto& spatial, auto& renderObject, auto& sprite) {
			auto materialResourceEntity = getMaterialResourceIfReadyToRender(registry, renderObject.materialResource);
			if (!materialResourceEntity) {
				return;
			}

			if (!materialBuffers.contains(*materialResourceEntity)) {
				return;
			}

			auto& quadBufferData{ materialBuffers[*materialResourceEntity] };
			const Material& materialResource = registry.get<Material>(*materialResourceEntity);
			extractRenderObjectToMaterialBuffer(registry, quadBufferData, spatial, renderObject, materialResource);
		});
	}

	void renderPerMaterialBuffers(entt::entity viewportEntity, entt::registry& registry, PerMaterialBuffersMap& materialBuffers) {
		for (auto& [materialEntity, materialBuffers] : materialBuffers) {
			if (materialEntity == entt::null || !registry.all_of<Material>(materialEntity)) {
				continue;
			}

			const Material& materialResource = registry.get<Material>(materialEntity);
			if (!registry.all_of<Core::ResourceHandle>(materialResource.textureImage) ||
				!registry.all_of<Core::ResourceHandle>(materialResource.shaderProgram)) {
				return;
			}

			const auto& textureImageResourceHandle = registry.get<Core::ResourceHandle>(materialResource.textureImage);
			if (textureImageResourceHandle.mResourceEntity == entt::null ||
				!registry.all_of<Image>(textureImageResourceHandle.mResourceEntity)) {
				return;
			}

			const auto& shaderProgramResourceHandle =
				registry.get<Core::ResourceHandle>(materialResource.shaderProgram);
			if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
				!registry.all_of<ShaderProgram>(shaderProgramResourceHandle.mResourceEntity)) {
				return;
			}

			// const auto& bgfxUniforms = registry.get<BGFXUniforms>(shaderProgramResourceHandle.mResourceEntity);

			// if (!bgfxUniforms.uniforms.contains("s_texColour") || !bgfxUniforms.uniforms.contains("u_alphaColour")) {
			// 	return;
			// }

			// bgfx::UniformHandle textureColourUniform{ bgfxUniforms.uniforms.at("s_texColour") };
			// bgfx::UniformHandle alphaColourUniform{ bgfxUniforms.uniforms.at("u_alphaColour") };

			RenderCommand renderCommand;
			renderCommand.viewportEntity = viewportEntity;
			renderCommand.shaderProgram = shaderProgramResourceHandle.mResourceEntity;

			renderCommand.vertexBuffer = registry.create();
			renderCommand.vertexCount = materialBuffers.quadCount * 4;
			registry.emplace<VertexBuffer>(renderCommand.vertexBuffer, std::move(materialBuffers.vertexBuffer));

			renderCommand.indexBuffer = registry.create();
			renderCommand.indexCount = materialBuffers.quadCount * 6;
			registry.emplace<IndexBuffer>(renderCommand.indexBuffer, std::move(materialBuffers.indexBuffer));

			renderCommand.renderPass = materialResource.renderPass;

			renderCommand.texture = textureImageResourceHandle.mResourceEntity;

			entt::entity renderCommandEntity = registry.create();
			registry.emplace<RenderCommand>(renderCommandEntity, std::move(renderCommand));
			// bgfx::setTexture(0, textureColourUniform, bgfxTexture.textureHandle);
		}
	}

} // namespace SpriteRenderSystemInternal

namespace Gfx {



	SpriteRenderSystem::SpriteRenderSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler} {

		mTickHandle = mScheduler.schedule([this]() {
			tickSystem(mRegistry);
		});
	}

	SpriteRenderSystem::~SpriteRenderSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void SpriteRenderSystem::tickSystem(entt::registry& registry) {
		using namespace SpriteRenderSystemInternal;
		auto materialBuffers = createPerMaterialBuffers(registry);
		extractRenderObjectsToPerMaterialBuffers(registry, materialBuffers);

		registry.view<Viewport>().each([&registry, &materialBuffers](entt::entity entity, const Viewport& viewport) {
			renderPerMaterialBuffers(entity, registry, materialBuffers);
		});
	}

} // namespace Gfx
