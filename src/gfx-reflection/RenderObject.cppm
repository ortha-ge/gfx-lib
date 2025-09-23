export module Gfx.Reflection.RenderObject;

export import Gfx.RenderObject;

import Core.EnTTClassAnnotation;
import Core.ReflectionContext;

template <>
void Core::reflect<Gfx::RenderObject>(ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<RenderObject>("RenderObject")
		.annotate(createEnTTClassAnnotation<RenderObject>())
		.build();
}