export module Gfx.Reflection.Tilemap;

export import Gfx.TilemapDescriptor;

import Core.Reflect;
import Glm.Reflection.Vec2;

template <>
void Core::reflect<Gfx::Tile>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<Tile>("Tile")
		.field<&Tile::coordinates>("coordinates")
		.field<&Tile::atlasIndex>("atlasIndex")
		;
}

template <>
void Core::reflect<Gfx::TilemapDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflect<Tile>(reflectionContext);

	reflectionContext.addClass<TilemapDescriptor>("TileMap")
		.field<&TilemapDescriptor::atlasFilePath>("atlasFilePath")
		.field<&TilemapDescriptor::dimensions>("dimensions")
		.field<&TilemapDescriptor::tiles>("tiles")
		;
}