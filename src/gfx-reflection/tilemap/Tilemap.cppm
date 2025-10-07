export module Ortha.Gfx.Reflection.Tilemap;

export import Ortha.Gfx.TilemapDescriptor;

import Ortha.Core.Reflect;
import Glm.Reflection.Vec2;

template <>
void Core::reflect<Ortha::Gfx::Tile>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;

	reflectionContext.addClass<Tile>("Tile")
		.field<&Tile::coordinates>("coordinates")
		.field<&Tile::atlasIndex>("atlasIndex")
		;
}

template <>
void Core::reflect<Ortha::Gfx::TilemapDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;

	reflect<Tile>(reflectionContext);

	reflectionContext.addClass<TilemapDescriptor>("TileMap")
		.field<&TilemapDescriptor::atlasFilePath>("atlasFilePath")
		.field<&TilemapDescriptor::dimensions>("dimensions")
		.field<&TilemapDescriptor::tiles>("tiles")
		;
}