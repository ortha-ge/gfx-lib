module;

#include <cstdint>
#include <utility>
#include <vector>

module Gfx.TilemapRenderSystem;

import Ortha.RTTI.Any;
import Core.GlobalSpatial;
import Core.ResourceHandleUtils;
import Core.Spatial;
import Ortha.RTTI.TypeHandle;
import Ortha.RTTI.TypeId;
import Gfx.Camera;
import Gfx.Image;
import Gfx.ImageAtlas;
import Gfx.IndexBuffer;
import Gfx.RenderCandidates;
import Gfx.RenderCommand;
import Gfx.RenderState;
import Gfx.ShaderProgram;
import Gfx.TextureCoordinates;
import Gfx.Tilemap;
import Gfx.TilemapObject;
import Gfx.VertexBuffer;
import Gfx.Viewport;
import entt;
import glm;

namespace Gfx::TilemapRenderSystemInternal {

	struct MappedTile {
		glm::ivec2 coordinates;
		TextureCoordinates textureCoordinates;
	};

	struct TilemapVertex {
		glm::vec3 position;
		glm::vec2 texCoords0;
	};

	void renderTilemap(
		entt::registry& registry, const Tilemap& tilemap, const ImageAtlas& atlas,
		const Core::GlobalSpatial& spatial, const entt::entity shaderProgramEntity, const entt::entity viewportEntity, const glm::mat4& viewMatrix) {
		using namespace Core;
		using namespace Gfx;

		auto&& [atlasImageEntity, atlasImage] = getResourceAndEntity<Image>(registry, atlas.image);
		if (!atlasImage) {
			return;
		}

		// TODO: This could be baked at load time.
		std::vector<MappedTile> mappedTiles;
		for (size_t x = 0; x < tilemap.dimensions.x; ++x) {
			for (size_t y = 0; y < tilemap.dimensions.y; ++y) {
				const auto& tile{ tilemap.tiles[x][y] };
				if (tile && (*tile) < atlas.regions.size()) {
					const auto& atlasRegion{ atlas.regions[*tile] };
					MappedTile mappedTile;
					mappedTile.coordinates = glm::ivec2{ x, y };
					mappedTile.textureCoordinates = atlasRegion.textureCoordinates;
					mappedTiles.emplace_back(std::move(mappedTile));
				}
			}
		}

		if (mappedTiles.empty()) {
			return;
		}

		const size_t vertexCount = mappedTiles.size() * 4;
		const size_t indexCount = mappedTiles.size() * 6;

		VertexBuffer vertexBuffer;
		vertexBuffer.vertexLayout = shaderProgramEntity;
		vertexBuffer.type = VertexBufferType::Transient;
		vertexBuffer.data.resize(sizeof(TilemapVertex) * vertexCount);

		IndexBuffer indexBuffer;
		indexBuffer.type = IndexBufferType::Transient;
		indexBuffer.data.resize(sizeof(uint16_t) * indexCount);

		for (size_t mappedTileIndex = 0; mappedTileIndex < mappedTiles.size(); ++mappedTileIndex) {
			const auto& mappedTile{ mappedTiles[mappedTileIndex] };
			const auto& texCoords{ mappedTile.textureCoordinates };

			const TextureCoordinates factoredCoords{
				{ texCoords.bottomLeft.x / atlasImage->width, texCoords.bottomLeft.y / atlasImage->height},
				{ texCoords.topRight.x / atlasImage->width, texCoords.topRight.y / atlasImage->height }
			};

			constexpr glm::vec2 tileSize{ 100.0f, 100.0f };
			const glm::vec2 tileBottomLeft{ glm::vec2{ mappedTile.coordinates} * tileSize };
			const glm::vec2 tileTopRight{ tileBottomLeft + tileSize };

			const size_t startVertex = mappedTileIndex * 4;
			auto* vertexHead = reinterpret_cast<TilemapVertex*>(&vertexBuffer.data[sizeof(TilemapVertex) * startVertex]);
			vertexHead[0] = { spatial.position + glm::vec3{ tileBottomLeft.x, tileBottomLeft.y, 0.0f }, { factoredCoords.bottomLeft.x, factoredCoords.bottomLeft.y } };
			vertexHead[1] = { spatial.position + glm::vec3{ tileBottomLeft.x, tileTopRight.y, 0.0f }, { factoredCoords.bottomLeft.x, factoredCoords.topRight.y } };
			vertexHead[2] = { spatial.position + glm::vec3{ tileTopRight.x, tileTopRight.y, 0.0f }, { factoredCoords.topRight.x, factoredCoords.topRight.y } };
			vertexHead[3] = { spatial.position + glm::vec3{ tileTopRight.x, tileBottomLeft.y, 0.0f }, { factoredCoords.topRight.x, factoredCoords.bottomLeft.y } };

			const size_t startIndex = mappedTileIndex * 6;
			auto* indexHead = reinterpret_cast<uint16_t*>(&indexBuffer.data[sizeof(uint16_t) * startIndex]);
			indexHead[0] = startVertex;
			indexHead[1] = startVertex + 1;
			indexHead[2] = startVertex + 3;

			indexHead[3] = startVertex + 3;
			indexHead[4] = startVertex + 1;
			indexHead[5] = startVertex + 2;
		}

		const entt::entity vertexBufferEntity = registry.create();
		registry.emplace<VertexBuffer>(vertexBufferEntity, std::move(vertexBuffer));

		const entt::entity indexBufferEntity = registry.create();
		registry.emplace<IndexBuffer>(indexBufferEntity, std::move(indexBuffer));

		RenderCommand renderCommand;
		renderCommand.vertexBuffer = vertexBufferEntity;
		renderCommand.vertexCount = vertexCount;

		renderCommand.indexBuffer = indexBufferEntity;
		renderCommand.indexCount = indexCount;

		renderCommand.shaderProgram = shaderProgramEntity;
		renderCommand.viewportEntity = viewportEntity;
		renderCommand.renderPass = 1;
		renderCommand.uniformData["s_texColour"] = Ortha::RTTI::Any(entt::entity{ atlasImageEntity });

		RenderState renderState{};
		renderState.bufferWriting = BufferWriting::RGB | BufferWriting::Alpha;
		renderState.blendLhs = BlendOperand::SourceAlpha;
		renderState.blendOperator = BlendOperator::Add;
		renderState.blendRhs = BlendOperand::InverseSourceAlpha;
		renderCommand.renderState = renderState;

		renderCommand.viewMatrix = viewMatrix;

		const entt::entity renderCommandEntity = registry.create();
		registry.emplace<RenderCommand>(renderCommandEntity, renderCommand);
	}

} // namespace Gfx::TilemapRenderSystemInternal

namespace Gfx {

	TilemapRenderSystem::TilemapRenderSystem(entt::registry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	TilemapRenderSystem::~TilemapRenderSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void TilemapRenderSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace TilemapRenderSystemInternal;

		registry.view<Camera>()
			.each([&registry](const entt::entity entity, const Camera&) {
				RenderCandidates& renderCandidates = registry.get_or_emplace<RenderCandidates>(entity);

				if (!renderCandidates.candidateBuckets.contains(Ortha::RTTI::TypeId::get<TilemapObject>())) {
					auto renderCandidateVisitor = [&registry](RenderCandidateBucket::EntityList& entityList, const entt::entity entity) {
						if (!registry.all_of<TilemapObject>(entity)) {
							return;
						}

						entityList.emplace_back(entity);
					};

					renderCandidates.candidateBuckets.emplace(Ortha::RTTI::TypeHandle::get<TilemapObject>(), renderCandidateVisitor);
				}
			});

		registry.view<Camera, Spatial, RenderCandidates>()
			.each([&registry](const entt::entity, const Camera& camera, const Spatial& cameraSpatial, const RenderCandidates& renderCandidates) {
				if (!renderCandidates.candidateBuckets.contains(Ortha::RTTI::TypeId::get<TilemapObject>())) {
					return;
				}

				glm::mat4 translation = glm::translate(glm::mat4(1.0f), cameraSpatial.position);
				glm::mat4 rotation = glm::mat4_cast(cameraSpatial.rotation);
				glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraSpatial.scale);
				glm::mat4 viewMatrix{ glm::inverse(translation * rotation * scale) };

				const auto& tilemapRenderCandidates{ renderCandidates.candidateBuckets.at(Ortha::RTTI::TypeId::get<TilemapObject>()) };
				for (auto&& tilemapEntity : tilemapRenderCandidates.entityList) {
					if (!registry.all_of<TilemapObject, GlobalSpatial>(tilemapEntity)) {
						continue;
					}

					const auto& [tilemapObject, spatial] = registry.get<TilemapObject, GlobalSpatial>(tilemapEntity);
					const auto* tilemap = getResource<Tilemap>(registry, tilemapObject.tilemap);
					if (!tilemap) {
						return;
					}

					const auto* atlas = getResource<ImageAtlas>(registry, tilemap->atlas);
					if (!atlas) {
						return;
					}

					auto&& [shaderProgramEntity, shaderProgram] = getResourceAndEntity<ShaderProgram>(registry, tilemapObject.shaderProgram);
					if (!shaderProgram) {
						return;
					}

					renderTilemap(registry, *tilemap, *atlas, spatial, shaderProgramEntity, camera.viewport, viewMatrix);
				}
			});
	}

} // namespace Gfx
