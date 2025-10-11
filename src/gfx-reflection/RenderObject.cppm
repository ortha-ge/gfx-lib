export module Ortha.Gfx.Reflection.RenderObject;

export import Ortha.Gfx.RenderObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template <>
	void reflect<Gfx::RenderObject>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<RenderObject>("RenderObject");
			//.annotate(createEnTTComponentAttribute<RenderObject>())
	}

}
