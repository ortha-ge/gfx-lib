export module Ortha.Gfx.Reflection.Viewport;

export import Ortha.Gfx.Viewport;
export import Ortha.Gfx.ViewportDescriptor;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;
import Glm.Reflection.Vec2;

template<>
void Core::reflect<Ortha::Gfx::ViewportDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<ViewportDescriptor>("ViewportDescriptor")
		.field<&ViewportDescriptor::offset>("offset")
		.field<&ViewportDescriptor::dimensions>("dimensions")
		;
}

template<>
void Core::reflect<Ortha::Gfx::Viewport>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<Viewport>("Viewport")
		.field<&Viewport::offset>("offset")
		.field<&Viewport::dimensions>("dimensions")
		//.annotate(createEnTTComponentAttribute<Viewport>())
		;
}
