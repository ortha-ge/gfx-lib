export module Ortha.Gfx.Reflection.Tilemap;

export import Ortha.Gfx.TilemapDescriptor;

import Ortha.Core.Reflect;
import Glm.Reflection.Vec2;

namespace Ortha::Core {

	template <>
	void reflect<Gfx::Tile>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<Tile>("Tile")
			.field<&Tile::coordinates>("coordinates")
			.field<&Tile::atlasIndex>("atlasIndex")
			;
	}

	template <>
	void reflect<Gfx::TilemapDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflect<Tile>(reflectionContext);

		reflectionContext.addClass<TilemapDescriptor>("TileMap")
			.field<&TilemapDescriptor::atlasFilePath>("atlasFilePath")
			.field<&TilemapDescriptor::dimensions>("dimensions")
			.field<&TilemapDescriptor::tiles>("tiles")
			;
	}

}
