module;

#include <memory>
#include <optional>
#include <utility>
#include <vector>

module Gfx.TilemapLoadSystem;

import Core.JsonTypeLoaderAdapter;
import Core.ProcessError;
import Core.ResourceLoadRequest;
import Core.TypeLoader;
import Gfx.ImageAtlas;
import Gfx.Tilemap;
import Gfx.TilemapDescriptor;
import Gfx.Reflection.ImageAtlas;

namespace Gfx {

	TilemapLoadSystem::TilemapLoadSystem(entt::registry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	TilemapLoadSystem::~TilemapLoadSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void TilemapLoadSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		registry.view<TilemapDescriptor>(entt::exclude<ProcessError, Tilemap>)
			.each([&registry](const entt::entity entity, const TilemapDescriptor& tilemapDescriptor) {
				auto loaderAdapter = std::make_shared<JsonTypeLoaderAdapter<ImageAtlasDescriptor>>();
				Tilemap tilemap;
				tilemap.atlas = ResourceLoadRequest::create<TypeLoader>(registry, tilemapDescriptor.atlasFilePath, std::move(loaderAdapter));
				tilemap.dimensions = tilemapDescriptor.dimensions;

				tilemap.tiles.resize(tilemapDescriptor.dimensions.x, std::vector<std::optional<uint16_t>>(tilemapDescriptor.dimensions.y));
				for (auto&& tile : tilemapDescriptor.tiles) {
					if (tile.coordinates.x < 0 || tile.coordinates.x >= tilemapDescriptor.dimensions.x ||
						tile.coordinates.y < 0 || tile.coordinates.y >= tilemapDescriptor.dimensions.y) {
						addProcessError(registry, entity, "Tile coordinate {{ {}, {} }} exceeds tilemap dimensions.", tile.coordinates.x, tile.coordinates.y);
						return;
					}

					tilemap.tiles[tile.coordinates.x][tile.coordinates.y] = tile.atlasIndex;
				}

				registry.emplace<Tilemap>(entity, std::move(tilemap));
			});
	}

} // namespace Gfx
