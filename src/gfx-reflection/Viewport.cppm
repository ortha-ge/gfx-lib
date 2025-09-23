export module Gfx.Reflection.Viewport;

export import Gfx.Viewport;
export import Gfx.ViewportDescriptor;

import Core.EnTTClassAnnotation;
import Core.ReflectionContext;
import Glm.Reflection.Vec2;

template<>
void Core::reflect<Gfx::ViewportDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<ViewportDescriptor>("ViewportDescriptor")
		.property("offset", &ViewportDescriptor::offset)
		.property("dimensions", &ViewportDescriptor::dimensions)
		.build();
}

template<>
void Core::reflect<Gfx::Viewport>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<Viewport>("Viewport")
		.property("offset", &Viewport::offset)
		.property("dimensions", &Viewport::dimensions)
		.annotate(createEnTTClassAnnotation<Viewport>())
		.build();
}
