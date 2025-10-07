module;

#include <map>
#include <memory>
#include <unordered_map>
#include <variant>

#include <tracy/Tracy.hpp>

module Gfx.SpriteRenderSystem;

import Ortha.RTTI.Any;
import Core.GlobalSpatial;
import Core.ResourceHandle;
import Core.ResourceHandleUtils;
import Core.Spatial;
import Ortha.RTTI.TypeId;
import Gfx.Camera;
import Gfx.RenderCandidates;
import Gfx.Image;
import Gfx.IndexBuffer;
import Gfx.Material;
import Gfx.MaterialDescriptor;
import Gfx.RenderCandidates;
import Gfx.RenderCommand;
import Gfx.RenderObject;
import Gfx.RenderState;
import Gfx.ShaderProgram;
import Gfx.ShaderProgramDescriptor;
import Gfx.Sprite;
import Gfx.SpriteObject;
import Gfx.TextureCoordinates;
import Gfx.VertexBuffer;
import Gfx.Viewport;
import entt;
import glm;

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

	MaterialBuffers createMaterialBuffers(const entt::entity shaderProgramEntity, const ShaderVertexLayoutDescriptor& vertexLayout) {
		constexpr uint32_t maxVertices = (32 << 10);
		constexpr uint32_t maxIndices = (32 << 10);
		VertexBuffer vertexBuffer;
		vertexBuffer.vertexLayout = shaderProgramEntity;
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
		MaterialBuffers& materialBuffers, const Core::GlobalSpatial& spatial, const RenderObject& renderObject,
		const SpriteObject& spriteObject, const Material& material, const Image& textureImage, const Sprite& sprite,
		const ShaderVertexLayoutDescriptor& vertexLayout) {

		// Use the entire image if the frames are empty
		glm::vec2 bottomLeft{ 0.0f, 0.0f };
		glm::vec2 topRight{ textureImage.width, textureImage.height };

		if (!sprite.descriptor.frames.empty()) {
			if (spriteObject.currentFrame >= sprite.descriptor.frames.size()) {
				return;
			}

			const auto& textureCoordinates{ sprite.descriptor.frames[spriteObject.currentFrame] };
			bottomLeft = textureCoordinates.bottomLeft;
			topRight = textureCoordinates.topRight;
		}

		const uint32_t startVertex = materialBuffers.quadCount * 4;
		const uint32_t startIndex = materialBuffers.quadCount * 6;
		if (startVertex + 4 >= materialBuffers.maxVertexCount || startIndex + 6 >= materialBuffers.maxIndexCount) {
			return;
		}

		const float halfQuadWidth = (topRight.x - bottomLeft.x) * spatial.scale.x * 0.5f;
		const float halfQuadHeight = (topRight.y - bottomLeft.y) * spatial.scale.y * 0.5f;

		const TextureCoordinates factoredCoords{
			{ bottomLeft.x / textureImage.width, bottomLeft.y / textureImage.height},
			{ topRight.x / textureImage.width, topRight.y / textureImage.height }
		};

		// Top-Left
		pushQuadVertexToVertexBuffer(
			materialBuffers.vertexBuffer.data.data(), startVertex, vertexLayout, { -halfQuadWidth, -halfQuadHeight },
			spatial.position, spatial.rotation, { factoredCoords.bottomLeft.x, factoredCoords.bottomLeft.y });

		// Bottom-Left
		pushQuadVertexToVertexBuffer(
			materialBuffers.vertexBuffer.data.data(), startVertex + 1, vertexLayout, { -halfQuadWidth, halfQuadHeight },
			spatial.position, spatial.rotation, { factoredCoords.bottomLeft.x, factoredCoords.topRight.y });

		// Bottom-Right
		pushQuadVertexToVertexBuffer(
			materialBuffers.vertexBuffer.data.data(), startVertex + 2, vertexLayout, { halfQuadWidth, halfQuadHeight },
			spatial.position, spatial.rotation, { factoredCoords.topRight.x, factoredCoords.topRight.y });

		// Top-Right
		pushQuadVertexToVertexBuffer(
			materialBuffers.vertexBuffer.data.data(), startVertex + 3, vertexLayout, { halfQuadWidth, -halfQuadHeight },
			spatial.position, spatial.rotation, { factoredCoords.topRight.x, factoredCoords.bottomLeft.y });

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
		entt::registry& registry, ZMaterialBucket& zBucket, const Core::GlobalSpatial& spatial,
		const RenderObject& renderObject, const SpriteObject& spriteObject, const entt::entity materialEntity,
		const Material& material, const Image& textureImage, const Sprite& sprite, const entt::entity shaderProgramEntity, const ShaderProgram& shaderProgram) {

		if (!zBucket.materialBuffers.contains(materialEntity)) {
			auto materialBuffers = createMaterialBuffers(shaderProgramEntity, shaderProgram.vertexLayout);
			const bool isVertexBufferValid = !materialBuffers.vertexBuffer.data.empty();
			const bool isIndexBufferValid = !materialBuffers.indexBuffer.data.empty();
			assert(isVertexBufferValid && isIndexBufferValid);
			if (!isVertexBufferValid || !isIndexBufferValid) {
				return;
			}

			zBucket.materialBuffers.emplace(materialEntity, std::move(materialBuffers));
		}

		auto& materialBuffers{ zBucket.materialBuffers[materialEntity] };
		pushSpriteToMaterialBuffers(
			materialBuffers, spatial, renderObject, spriteObject, material, textureImage, sprite, shaderProgram.vertexLayout);
	}

	void pushSpriteToZBucketMap(
		entt::registry& registry, ZMaterialBucketMap& zBucketMap, const entt::entity entity, const Core::GlobalSpatial& spatial,
		const RenderObject& renderObject, const SpriteObject& spriteObject) {

		entt::entity materialEntity = entt::null;
		const Material* material = nullptr;
		if (std::holds_alternative<std::shared_ptr<Core::ResourceHandle>>(renderObject.materialResource)) {
			auto& resourceHandle{ std::get<std::shared_ptr<Core::ResourceHandle>>(renderObject.materialResource) };
			auto&& [_materialEntity, _material] = Core::getResourceAndEntity<Material>(registry, resourceHandle);
			material = _material;
			materialEntity = _materialEntity;
		} else if (std::holds_alternative<MaterialDescriptor>(renderObject.materialResource)) {
			material = registry.try_get<Material>(entity);
			materialEntity = entity;
		}

		if (!material || materialEntity == entt::null) {
			return;
		}

		auto&& [shaderProgramEntity, shaderProgram] = Core::getResourceAndEntity<ShaderProgram>(registry, material->shaderProgram);
		if (!shaderProgram) {
			return;
		}

		const auto&& [textureEntity, texture] = Core::getResourceAndEntity<Image>(registry, material->textureImage);
		if (!texture) {
			return;
		}

		const Sprite* sprite = nullptr;
		if (std::holds_alternative<std::shared_ptr<Core::ResourceHandle>>(spriteObject.spriteResource)) {
			const auto& spriteResource{ std::get<std::shared_ptr<Core::ResourceHandle>>(spriteObject.spriteResource) };
			sprite = Core::getResource<Sprite>(registry, spriteResource);
		} else {
			sprite = &std::get<Sprite>(spriteObject.spriteResource);
		}

		if (!sprite) {
			return;
		}

		if (!zBucketMap.contains(spatial.position.z)) {
			zBucketMap.emplace(spatial.position.z, ZMaterialBucket{});
		}

		pushSpriteToZBucket(
			registry, zBucketMap[spatial.position.z], spatial, renderObject, spriteObject, materialEntity, *material, *texture,
			*sprite, shaderProgramEntity, *shaderProgram);
	}

	ZMaterialBucketMap prepareSpriteZBucketMap(entt::registry& registry, const std::vector<entt::entity>& entities) {
		ZMaterialBucketMap zBucketMap;

		for (auto&& entity : entities) {
			if (!registry.all_of<Core::GlobalSpatial, RenderObject, SpriteObject>(entity)) {
				continue;
			}

			const auto& [spatial, renderObject, spriteObject] = registry.get<Core::GlobalSpatial, RenderObject, SpriteObject>(entity);
			pushSpriteToZBucketMap(registry, zBucketMap, entity, spatial, renderObject, spriteObject);
		}

		return zBucketMap;
	}

	void renderMaterialBuffers(
		entt::registry& registry, const Camera& camera, const glm::mat4& viewMatrix, entt::entity materialEntity,
		const MaterialBuffers& materialBuffers, uint32_t sortDepth) {

		if (materialEntity == entt::null || !registry.all_of<Material>(materialEntity)) {
			return;
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
		renderCommand.viewportEntity = camera.viewport;
		renderCommand.shaderProgram = shaderProgramEntity;

		renderCommand.vertexBuffer = registry.create();
		renderCommand.vertexCount = materialBuffers.quadCount * 4;
		registry.emplace<VertexBuffer>(renderCommand.vertexBuffer, materialBuffers.vertexBuffer);

		renderCommand.indexBuffer = registry.create();
		renderCommand.indexCount = materialBuffers.quadCount * 6;
		registry.emplace<IndexBuffer>(renderCommand.indexBuffer, materialBuffers.indexBuffer);

		renderCommand.sortDepth = sortDepth;

		renderCommand.uniformData["s_texColour"] = Ortha::RTTI::Any(entt::entity{ textureEntity });
		renderCommand.uniformData["u_alphaColour"] = Ortha::RTTI::Any(material.alphaColour.value_or(glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f }));

		RenderState renderState{};
		renderState.bufferWriting = BufferWriting::RGB | BufferWriting::Alpha;
		renderState.blendLhs = BlendOperand::SourceAlpha;
		renderState.blendOperator = BlendOperator::Add;
		renderState.blendRhs = BlendOperand::InverseSourceAlpha;
		renderCommand.renderState = renderState;

		renderCommand.viewMatrix = viewMatrix;

		entt::entity renderCommandEntity = registry.create();
		registry.emplace<RenderCommand>(renderCommandEntity, renderCommand);
	}

	void renderSpriteZBucket(
		entt::registry& registry, const Camera& camera, const glm::mat4& viewMatrix, const ZMaterialBucket& zBucket, uint32_t sortDepth) {
		for (auto&& [materialEntity, materialBuffers] : zBucket.materialBuffers) {
			renderMaterialBuffers(registry, camera, viewMatrix, materialEntity, materialBuffers, sortDepth);
		}
	}

	void renderSpriteZBucketMap(
		entt::registry& registry, const Camera& camera, const glm::mat4& viewMatrix, const ZMaterialBucketMap& zBucketMap) {
		uint32_t sortDepth = 0u;
		for (auto&& [_, zBucket] : zBucketMap) {
			renderSpriteZBucket(registry, camera, viewMatrix, zBucket, sortDepth++);
		}
	}

} // namespace Gfx::SpriteRenderSystemInternal

namespace Gfx {

	SpriteRenderSystem::SpriteRenderSystem(entt::registry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	SpriteRenderSystem::~SpriteRenderSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void SpriteRenderSystem::tickSystem(entt::registry& registry) {
		using namespace Core;

		registry.view<Camera>()
			.each([&registry](const entt::entity entity, const Camera&) {
				RenderCandidates& renderCandidates = registry.get_or_emplace<RenderCandidates>(entity);

				if (!renderCandidates.candidateBuckets.contains(Ortha::RTTI::TypeId::get<SpriteObject>())) {
					auto renderCandidateVisitor = [&registry](RenderCandidateBucket::EntityList& entityList, const entt::entity entity) {
						if (!registry.all_of<SpriteObject>(entity)) {
							return;
						}

						entityList.emplace_back(entity);
					};

					renderCandidates.candidateBuckets.emplace(Ortha::RTTI::TypeId::get<SpriteObject>(), renderCandidateVisitor);
				}
			});

		ZoneScopedN("SpriteRenderSystem::tickSystem");
		registry.view<RenderObject>(entt::exclude<Material, MaterialDescriptor>)
			.each([&registry](const entt::entity entity, const RenderObject& renderObject) {
				if (!std::holds_alternative<MaterialDescriptor>(renderObject.materialResource)) {
					return;
				}

				registry.emplace<MaterialDescriptor>(entity, std::get<MaterialDescriptor>(renderObject.materialResource));
			});

		using namespace SpriteRenderSystemInternal;
		registry.view<Camera, Spatial, RenderCandidates>()
			.each([&registry](const entt::entity, const Camera& camera, const Spatial& cameraSpatial, const RenderCandidates& renderCandidates) {
				if (!renderCandidates.candidateBuckets.contains(Ortha::RTTI::TypeId::get<SpriteObject>())) {
					return;
				}

				glm::mat4 translation = glm::translate(glm::mat4(1.0f), cameraSpatial.position);
				glm::mat4 rotation = glm::mat4_cast(cameraSpatial.rotation);
				glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraSpatial.scale);
				glm::mat4 viewMatrix{ glm::inverse(translation * rotation * scale) };

				const auto& spriteRenderCandidates{ renderCandidates.candidateBuckets.at(Ortha::RTTI::TypeId::get<SpriteObject>()) };

				ZMaterialBucketMap zBucketMap{ prepareSpriteZBucketMap(registry, spriteRenderCandidates.entityList) };
				renderSpriteZBucketMap(registry, camera, viewMatrix, zBucketMap);
			});
	}

} // namespace Gfx
