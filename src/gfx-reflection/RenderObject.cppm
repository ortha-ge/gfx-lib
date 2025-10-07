export module Ortha.Gfx.Reflection.RenderObject;

export import Ortha.Gfx.RenderObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;

namespace Ortha::Core {

	template <>
	void reflect<Gfx::RenderObject>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<RenderObject>("RenderObject")
			//.annotate(createEnTTComponentAttribute<RenderObject>())
			;
	}

}
