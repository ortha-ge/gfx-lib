export module Gfx.Reflection.RenderObject;

export import Gfx.RenderObject;

import Core.EnTTComponentAttribute;
import Core.ReflectionContext;

template <>
void Core::reflect<Gfx::RenderObject>(ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<RenderObject>("RenderObject")
		.annotate(createEnTTComponentAttribute<RenderObject>())
		.build();
}