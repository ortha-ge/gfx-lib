export module Ortha.Gfx.Reflection.TilemapObject;

export import Ortha.Gfx.TilemapObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template <>
	void reflect<Gfx::TilemapObject>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<TilemapObject>("TilemapObject")
			//.annotate(createEnTTComponentAttribute<TilemapObject>())
			;
	}

}