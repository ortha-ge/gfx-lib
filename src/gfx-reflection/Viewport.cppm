export module Gfx.Reflection.Viewport;

export import Gfx.ViewportDescriptor;

import Core.ClassReflection;
import Core.ReflectionContext;

template<>
void Core::reflect<Gfx::ViewportDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<Gfx::ViewportDescriptor>("Viewport")
		.property("x", &ViewportDescriptor::x)
		.property("y", &ViewportDescriptor::y)
		.property("width", &ViewportDescriptor::width)
		.property("height", &ViewportDescriptor::height)
		.build();
}
