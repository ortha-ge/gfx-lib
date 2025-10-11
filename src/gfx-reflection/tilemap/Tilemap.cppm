export module Ortha.Gfx.Reflection.Tilemap;

export import Ortha.Gfx.TilemapDescriptor;

import Glm.Reflection.Vec2;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template <>
	void reflect<Gfx::Tile>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<Tile>("Tile")
			.field<&Tile::coordinates>("coordinates")
			.field<&Tile::atlasIndex>("atlasIndex");
	}

	template <>
	void reflect<Gfx::TilemapDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		ensureReflected<Tile>(reflectionContext);
		reflectionContext.addClass<TilemapDescriptor>("TileMap")
			.field<&TilemapDescriptor::atlasFilePath>("atlasFilePath")
			.field<&TilemapDescriptor::dimensions>("dimensions")
			.field<&TilemapDescriptor::tiles>("tiles");
	}

}
