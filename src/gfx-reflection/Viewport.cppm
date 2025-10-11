export module Ortha.Gfx.Reflection.Viewport;

export import Ortha.Gfx.Viewport;
export import Ortha.Gfx.ViewportDescriptor;

import Glm.Reflection.Vec2;
import Ortha.Core.EnTTComponentAttribute;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::ViewportDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<ViewportDescriptor>("ViewportDescriptor")
			.field<&ViewportDescriptor::offset>("offset")
			.field<&ViewportDescriptor::dimensions>("dimensions");
	}

	template<>
	void reflect<Gfx::Viewport>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<Viewport>("Viewport")
			.field<&Viewport::offset>("offset")
			.field<&Viewport::dimensions>("dimensions");
			//.annotate(createEnTTComponentAttribute<Viewport>())
	}

}
