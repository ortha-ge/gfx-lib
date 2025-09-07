module;

#include <entt/entt.hpp>

module Gfx.SpriteRenderSystem;

import Core.Any;
import Core.ResourceHandle;
import Core.Spatial;
import Gfx.Colour;
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

	using MaterialBuffersMap = std::unordered_map<entt::entity, MaterialBuffers>;

	struct ZMaterialBucket {
		MaterialBuffersMap materialBuffers;
	};

	using ZMaterialBucketMap = std::map<float, ZMaterialBucket>;

	MaterialBuffers createMaterialBuffers(const ShaderVertexLayoutDescriptor& vertexLayout) {
		constexpr uint32_t maxVertices = (32 << 10);
		constexpr uint32_t maxIndices = (32 << 10);
		VertexBuffer vertexBuffer;
		vertexBuffer.type = VertexBufferType::Transient;
		vertexBuffer.data.resize(getBufferSizeForVertexLayout(vertexLayout, maxVertices));

		IndexBuffer indexBuffer;
		indexBuffer.is32Bit = maxVertices > std::numeric_limits<uint16_t>::max();

		const size_t indexElementSize = indexBuffer.is32Bit ? sizeof(uint32_t) : sizeof(uint16_t);
		indexBuffer.data.resize(indexElementSize * maxIndices);

		return MaterialBuffers{ std::move(vertexBuffer), std::move(indexBuffer), maxVertices, maxIndices };
	}

	void pushSpriteToMaterialBuffers(
		MaterialBuffers& materialBuffers, const Core::Spatial& spatial, const RenderObject& renderObject,
		const Material& material, const ShaderVertexLayoutDescriptor& vertexLayout) {

		if (renderObject.currentSpriteFrame >= material.spriteFrames.size()) {
			return;
		}

		const auto& [x0, y0, x1, y1]{ material.spriteFrames[renderObject.currentSpriteFrame] };
		const uint32_t startVertex = materialBuffers.quadCount * 4;
		const uint32_t startIndex = materialBuffers.quadCount * 6;
		if (startVertex + 4 >= materialBuffers.maxVertexCount || startIndex + 6 >= materialBuffers.maxIndexCount) {
			return;
		}

		const float halfQuadWidth = (x1 - x0) * spatial.scaleX * 0.5f;
		const float halfQuadHeight = (y1 - y0) * spatial.scaleY * 0.5f;

		const TextureCoordinates factoredCoords{ x0 / material.width, y0 / material.height, x1 / material.width,
												 y1 / material.height };

		for (size_t i = 0; i < 4; ++i) {
			uint8_t* vertexHead =
				materialBuffers.vertexBuffer.data.data() + getStrideForVertexLayout(vertexLayout) * (startVertex + i);
			auto* positionHead = reinterpret_cast<float*>(
				vertexHead + getOffsetForVertexLayoutAttribute(vertexLayout, ShaderVertexLayoutAttributeId::Position));
			auto* texCoordHead = reinterpret_cast<float*>(
				vertexHead + getOffsetForVertexLayoutAttribute(vertexLayout, ShaderVertexLayoutAttributeId::TexCoord0));

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

	void pushSpriteToZBucket(
		entt::registry& registry, ZMaterialBucket& zBucket, const Core::Spatial& spatial,
		const RenderObject& renderObject, const entt::entity materialEntity, const Material& material,
		const ShaderProgram& shaderProgram) {

		if (!zBucket.materialBuffers.contains(materialEntity)) {
			zBucket.materialBuffers.emplace(materialEntity, createMaterialBuffers(shaderProgram.vertexLayout));
		}

		auto& materialBuffers{ zBucket.materialBuffers[materialEntity] };
		pushSpriteToMaterialBuffers(materialBuffers, spatial, renderObject, material, shaderProgram.vertexLayout);
	}

	void pushSpriteToZBucketMap(
		entt::registry& registry, ZMaterialBucketMap& zBucketMap, const Core::Spatial& spatial,
		const RenderObject& renderObject) {

		if (!registry.all_of<Core::ResourceHandle>(renderObject.materialResource)) {
			return;
		}

		const auto& materialResourceHandle{ registry.get<Core::ResourceHandle>(renderObject.materialResource) };
		if (!registry.all_of<Material>(materialResourceHandle.mResourceEntity)) {
			return;
		}

		const auto& material{ registry.get<Material>(materialResourceHandle.mResourceEntity) };
		if (!registry.all_of<Core::ResourceHandle>(material.shaderProgram)) {
			return;
		}

		const auto& shaderProgramResourceHandle{ registry.get<Core::ResourceHandle>(material.shaderProgram) };
		if (!registry.all_of<ShaderProgram>(shaderProgramResourceHandle.mResourceEntity)) {
			return;
		}

		const auto& shaderProgram{ registry.get<ShaderProgram>(shaderProgramResourceHandle.mResourceEntity) };

		if (!zBucketMap.contains(spatial.z)) {
			zBucketMap.emplace(spatial.z, ZMaterialBucket{});
		}

		pushSpriteToZBucket(
			registry, zBucketMap[spatial.z], spatial, renderObject, materialResourceHandle.mResourceEntity, material,
			shaderProgram);
	}

	ZMaterialBucketMap prepareSpriteZBucketMap(entt::registry& registry) {
		ZMaterialBucketMap zBucketMap;

		registry.view<Core::Spatial, RenderObject, Sprite>().each(
			[&registry, &zBucketMap](auto& spatial, auto renderObject, auto& sprite) {
				pushSpriteToZBucketMap(registry, zBucketMap, spatial, renderObject);
			});

		return zBucketMap;
	}

	void renderMaterialBuffers(
		entt::registry& registry, entt::entity viewportEntity, entt::entity materialEntity,
		const MaterialBuffers& materialBuffers, uint16_t renderPass) {

		if (materialEntity == entt::null || !registry.all_of<Material>(materialEntity)) {
			return;
			;
		}

		const Material& material = registry.get<Material>(materialEntity);
		if (!registry.all_of<Core::ResourceHandle>(material.textureImage) ||
			!registry.all_of<Core::ResourceHandle>(material.shaderProgram)) {
			return;
		}

		const auto& textureImageResourceHandle = registry.get<Core::ResourceHandle>(material.textureImage);
		if (textureImageResourceHandle.mResourceEntity == entt::null ||
			!registry.all_of<Image>(textureImageResourceHandle.mResourceEntity)) {
			return;
		}

		const auto& shaderProgramResourceHandle = registry.get<Core::ResourceHandle>(material.shaderProgram);
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
		registry.emplace<VertexBuffer>(renderCommand.vertexBuffer, materialBuffers.vertexBuffer);

		renderCommand.indexBuffer = registry.create();
		renderCommand.indexCount = materialBuffers.quadCount * 6;
		registry.emplace<IndexBuffer>(renderCommand.indexBuffer, materialBuffers.indexBuffer);

		renderCommand.renderPass = renderPass;

		renderCommand.uniformData["s_texColour"] = Core::Any(entt::entity{ textureImageResourceHandle.mResourceEntity });
		renderCommand.uniformData["u_alphaColour"] = Core::Any(material.alphaColour.value_or(Colour{ 0.0f, 0.0f, 0.0f, 0.0f }));

		entt::entity renderCommandEntity = registry.create();
		registry.emplace<RenderCommand>(renderCommandEntity, renderCommand);
	}

	void renderSpriteZBucket(
		entt::registry& registry, entt::entity viewportEntity, const ZMaterialBucket& zBucket, uint16_t& renderPass) {
		for (auto&& [materialEntity, materialBuffers] : zBucket.materialBuffers) {
			renderMaterialBuffers(registry, viewportEntity, materialEntity, materialBuffers, renderPass++);
		}
	}

	void renderSpriteZBucketMap(
		entt::registry& registry, entt::entity viewportEntity, const ZMaterialBucketMap& zBucketMap) {
		uint16_t renderPass{ 0u };
		for (auto&& [z, zBucket] : zBucketMap) {
			renderSpriteZBucket(registry, viewportEntity, zBucket, renderPass);
		}
	}

} // namespace Gfx::SpriteRenderSystemInternal

namespace Gfx {

	SpriteRenderSystem::SpriteRenderSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	SpriteRenderSystem::~SpriteRenderSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void SpriteRenderSystem::tickSystem(entt::registry& registry) {
		using namespace SpriteRenderSystemInternal;
		ZMaterialBucketMap zBucketMap{ prepareSpriteZBucketMap(registry) };

		registry.view<Viewport>().each([&registry, &zBucketMap](entt::entity entity, const Viewport& viewport) {
			renderSpriteZBucketMap(registry, entity, zBucketMap);
		});
	}

} // namespace Gfx
