module;

#include <cstdint>
#include <optional>
#include <vector>

#include <glm/vec2.hpp>

export module Gfx.Tilemap;

export import Gfx.TilemapDescriptor;

import Core.ResourceHandle;

export namespace Gfx {

	struct Tilemap {
		Core::ResourceHandlePtr atlas;
		glm::ivec2 dimensions{};
		std::vector<std::vector<std::optional<uint16_t>>> tiles;
	};

} // namespace Gfx
