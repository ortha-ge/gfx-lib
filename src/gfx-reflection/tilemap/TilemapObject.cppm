export module Gfx.Reflection.TilemapObject;

export import Gfx.TilemapObject;

import Core.EnTTComponentAttribute;
import Core.Reflect;

template <>
void Core::reflect<Gfx::TilemapObject>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<TilemapObject>("TilemapObject")
		//.annotate(createEnTTComponentAttribute<TilemapObject>())
		;
}