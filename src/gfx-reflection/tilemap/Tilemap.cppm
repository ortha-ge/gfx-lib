export module Gfx.Reflection.Tilemap;

export import Gfx.TilemapDescriptor;

import Core.ReflectionContext;
import Glm.Reflection.Vec2;

template <>
void Core::reflect<Gfx::Tile>(ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<Tile>("Tile")
		.property("coordinates", &Tile::coordinates)
		.property("atlasIndex", &Tile::atlasIndex)
		.build();
}

template <>
void Core::reflect<Gfx::TilemapDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<TilemapDescriptor>("TileMap")
		.property("atlasFilePath", &TilemapDescriptor::atlasFilePath)
		.property("dimensions", &TilemapDescriptor::dimensions)
		.property("tiles", &TilemapDescriptor::tiles)
		.build();
}