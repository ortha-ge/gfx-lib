export module Gfx.Reflection.TilemapObject;

export import Gfx.TilemapObject;

import Core.EnTTComponentAttribute;
import Core.ReflectionContext;

template <>
void Core::reflect<Gfx::TilemapObject>(ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<TilemapObject>("TilemapObject")
		.annotate(createEnTTComponentAttribute<TilemapObject>())
		.build();
}