export module Ortha.Gfx.Reflection.RenderObject;

export import Ortha.Gfx.RenderObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;

template <>
void Core::reflect<Ortha::Gfx::RenderObject>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;

	reflectionContext.addClass<RenderObject>("RenderObject")
		//.annotate(createEnTTComponentAttribute<RenderObject>())
		;
}