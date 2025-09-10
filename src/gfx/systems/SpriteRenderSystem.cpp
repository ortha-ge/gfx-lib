module;

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
import Gfx.SpriteObject;
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

	void pushQuadVertexToVertexBuffer(
		uint8_t* vertexBuffer, uint32_t bufferIndex, const ShaderVertexLayoutDescriptor& vertexLayout,
		const glm::vec2& quadVertex, const glm::vec3& position, const glm::fquat& rotation, const glm::vec2& texCoord) {

		uint8_t* vertexHead = vertexBuffer + getStrideForVertexLayout(vertexLayout) * bufferIndex;
		auto* positionHead = reinterpret_cast<glm::vec3*>(
			vertexHead + getOffsetForVertexLayoutAttribute(vertexLayout, ShaderVertexLayoutAttributeId::Position));
		auto* texCoordHead = reinterpret_cast<glm::vec2*>(
			vertexHead + getOffsetForVertexLayoutAttribute(vertexLayout, ShaderVertexLayoutAttributeId::TexCoord0));

		glm::vec3 rotatedVertex = rotation * glm::vec3{ quadVertex.x, quadVertex.y, 0.0f };
		*positionHead = position + rotatedVertex;
		*texCoordHead = texCoord;
	}

	void pushSpriteToMaterialBuffers(
		MaterialBuffers& materialBuffers, const Core::Spatial& spatial, const RenderObject& renderObject,
		const SpriteObject& spriteObject, const Material& material, const Sprite& sprite,
		const ShaderVertexLayoutDescriptor& vertexLayout) {

		if (spriteObject.currentFrame >= sprite.frames.size()) {
			return;
		}

		const auto& [x0, y0, x1, y1]{ sprite.frames[spriteObject.currentFrame] };
		const uint32_t startVertex = materialBuffers.quadCount * 4;
		const uint32_t startIndex = materialBuffers.quadCount * 6;
		if (startVertex + 4 >= materialBuffers.maxVertexCount || startIndex + 6 >= materialBuffers.maxIndexCount) {
			return;
		}

		const float halfQuadWidth = (x1 - x0) * spatial.scale.x * 0.5f;
		const float halfQuadHeight = (y1 - y0) * spatial.scale.y * 0.5f;

		const TextureCoordinates factoredCoords{ x0 / material.width, y0 / material.height, x1 / material.width,
												 y1 / material.height };

		// Top-Left
		pushQuadVertexToVertexBuffer(
			materialBuffers.vertexBuffer.data.data(), startVertex, vertexLayout, { -halfQuadWidth, -halfQuadHeight },
			spatial.position, spatial.rotation, { factoredCoords.x0, factoredCoords.y0 });

		// Bottom-Left
		pushQuadVertexToVertexBuffer(
			materialBuffers.vertexBuffer.data.data(), startVertex + 1, vertexLayout, { -halfQuadWidth, halfQuadHeight },
			spatial.position, spatial.rotation, { factoredCoords.x0, factoredCoords.y1 });

		// Bottom-Right
		pushQuadVertexToVertexBuffer(
			materialBuffers.vertexBuffer.data.data(), startVertex + 2, vertexLayout, { halfQuadWidth, halfQuadHeight },
			spatial.position, spatial.rotation, { factoredCoords.x1, factoredCoords.y1 });

		// Top-Right
		pushQuadVertexToVertexBuffer(
			materialBuffers.vertexBuffer.data.data(), startVertex + 3, vertexLayout, { halfQuadWidth, -halfQuadHeight },
			spatial.position, spatial.rotation, { factoredCoords.x1, factoredCoords.y0 });

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
		const RenderObject& renderObject, const SpriteObject& spriteObject, const entt::entity materialEntity,
		const Material& material, const Sprite& sprite, const ShaderProgram& shaderProgram) {

		if (!zBucket.materialBuffers.contains(materialEntity)) {
			zBucket.materialBuffers.emplace(materialEntity, createMaterialBuffers(shaderProgram.vertexLayout));
		}

		auto& materialBuffers{ zBucket.materialBuffers[materialEntity] };
		pushSpriteToMaterialBuffers(
			materialBuffers, spatial, renderObject, spriteObject, material, sprite, shaderProgram.vertexLayout);
	}

	void pushSpriteToZBucketMap(
		entt::registry& registry, ZMaterialBucketMap& zBucketMap, const Core::Spatial& spatial,
		const RenderObject& renderObject, const SpriteObject& spriteObject) {

		const auto&& [materialEntity, material] =
			Core::getResourceAndEntity<Material>(registry, renderObject.materialResource);
		if (!material) {
			return;
		}

		const auto* shaderProgram = Core::getResource<ShaderProgram>(registry, material->shaderProgram);
		if (!shaderProgram) {
			return;
		}

		const auto&& [spriteEntity, sprite] = Core::getResourceAndEntity<Sprite>(registry, spriteObject.spriteResource);
		if (!sprite) {
			return;
		}

		if (!zBucketMap.contains(spatial.position.z)) {
			zBucketMap.emplace(spatial.position.z, ZMaterialBucket{});
		}

		pushSpriteToZBucket(
			registry, zBucketMap[spatial.position.z], spatial, renderObject, spriteObject, materialEntity, *material,
			*sprite, *shaderProgram);
	}

	ZMaterialBucketMap prepareSpriteZBucketMap(entt::registry& registry) {
		ZMaterialBucketMap zBucketMap;

		registry.view<Core::Spatial, RenderObject, SpriteObject>().each(
			[&registry, &zBucketMap](auto& spatial, auto renderObject, auto& spriteObject) {
				pushSpriteToZBucketMap(registry, zBucketMap, spatial, renderObject, spriteObject);
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
		if (!material.textureImage || !material.shaderProgram) {
			return;
		}

		const auto&& [textureEntity, texture] = Core::getResourceAndEntity<Image>(registry, material.textureImage);
		if (!texture) {
			return;
		}

		const auto&& [shaderProgramEntity, shaderProgram] = Core::getResourceAndEntity<ShaderProgram>(registry, material.shaderProgram);
		if (!shaderProgram) {
			return;
		}

		RenderCommand renderCommand;
		renderCommand.viewportEntity = viewportEntity;
		renderCommand.shaderProgram = shaderProgramEntity;

		renderCommand.vertexBuffer = registry.create();
		renderCommand.vertexCount = materialBuffers.quadCount * 4;
		registry.emplace<VertexBuffer>(renderCommand.vertexBuffer, materialBuffers.vertexBuffer);

		renderCommand.indexBuffer = registry.create();
		renderCommand.indexCount = materialBuffers.quadCount * 6;
		registry.emplace<IndexBuffer>(renderCommand.indexBuffer, materialBuffers.indexBuffer);

		renderCommand.renderPass = renderPass;

		renderCommand.uniformData["s_texColour"] = Core::Any(entt::entity{ textureEntity });
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
