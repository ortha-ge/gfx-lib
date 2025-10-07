module;

#include <string>
#include <vector>

#include <glm/vec2.hpp>

export module Ortha.Gfx.TilemapDescriptor;

export namespace Ortha::Gfx {

	struct Tile {
		glm::ivec2 coordinates{};
		uint16_t atlasIndex{};
	};

	struct TilemapDescriptor {
		std::string atlasFilePath;
		glm::ivec2 dimensions{};
		std::vector<Tile> tiles;
	};

}