export module Ortha.Gfx.Reflection.Viewport;

export import Ortha.Gfx.Viewport;
export import Ortha.Gfx.ViewportDescriptor;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;
import Glm.Reflection.Vec2;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::ViewportDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<ViewportDescriptor>("ViewportDescriptor")
			.field<&ViewportDescriptor::offset>("offset")
			.field<&ViewportDescriptor::dimensions>("dimensions")
			;
	}

	template<>
	void reflect<Gfx::Viewport>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<Viewport>("Viewport")
			.field<&Viewport::offset>("offset")
			.field<&Viewport::dimensions>("dimensions")
			//.annotate(createEnTTComponentAttribute<Viewport>())
			;
	}

}
