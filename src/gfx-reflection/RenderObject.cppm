export module Gfx.Reflection.RenderObject;

export import Gfx.RenderObject;

import Core.EnTTComponentAttribute;
import Core.Reflect;

template <>
void Core::reflect<Gfx::RenderObject>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<RenderObject>("RenderObject")
		//.annotate(createEnTTComponentAttribute<RenderObject>())
		;
}