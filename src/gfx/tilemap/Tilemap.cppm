module;

#include <cstdint>
#include <optional>
#include <vector>

#include <glm/vec2.hpp>

export module Ortha.Gfx.Tilemap;

export import Ortha.Gfx.TilemapDescriptor;

import Ortha.Core.ResourceHandle;

export namespace Ortha::Gfx {

	struct Tilemap {
		Core::ResourceHandlePtr atlas;
		glm::ivec2 dimensions{};
		std::vector<std::vector<std::optional<uint16_t>>> tiles;
	};

} // namespace Ortha::Gfx
