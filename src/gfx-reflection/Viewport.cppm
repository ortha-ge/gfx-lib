export module Gfx.Reflection.Viewport;

export import Gfx.ViewportDescriptor;

import Core.ReflectionContext;
import Glm.Reflection.Vec2;

template<>
void Core::reflect<Gfx::ViewportDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<Gfx::ViewportDescriptor>("Viewport")
		.property("offset", &ViewportDescriptor::offset)
		.property("dimensions", &ViewportDescriptor::dimensions)
		.build();
}
