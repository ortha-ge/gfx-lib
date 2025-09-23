export module Gfx.Reflection.FontObject;

export import Gfx.FontObject;

import Core.EnTTClassAnnotation;
import Core.ReflectionContext;

template <>
void Core::reflect<Gfx::FontObject>(ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<FontObject>("FontObject")
		.property("text", &FontObject::text)
		.annotate(createEnTTClassAnnotation<FontObject>())
		.build();
}