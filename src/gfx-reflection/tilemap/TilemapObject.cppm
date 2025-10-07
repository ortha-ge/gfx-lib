export module Ortha.Gfx.Reflection.TilemapObject;

export import Ortha.Gfx.TilemapObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;

template <>
void Core::reflect<Ortha::Gfx::TilemapObject>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;

	reflectionContext.addClass<TilemapObject>("TilemapObject")
		//.annotate(createEnTTComponentAttribute<TilemapObject>())
		;
}