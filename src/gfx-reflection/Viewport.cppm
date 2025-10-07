export module Gfx.Reflection.Viewport;

export import Gfx.Viewport;
export import Gfx.ViewportDescriptor;

import Core.EnTTComponentAttribute;
import Core.Reflect;
import Glm.Reflection.Vec2;

template<>
void Core::reflect<Gfx::ViewportDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<ViewportDescriptor>("ViewportDescriptor")
		.field<&ViewportDescriptor::offset>("offset")
		.field<&ViewportDescriptor::dimensions>("dimensions")
		;
}

template<>
void Core::reflect<Gfx::Viewport>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<Viewport>("Viewport")
		.field<&Viewport::offset>("offset")
		.field<&Viewport::dimensions>("dimensions")
		//.annotate(createEnTTComponentAttribute<Viewport>())
		;
}
