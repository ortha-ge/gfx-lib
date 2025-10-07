export module Ortha.Gfx.Reflection.TilemapObject;

export import Ortha.Gfx.TilemapObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;

namespace Ortha::Core {

	template <>
	void reflect<Gfx::TilemapObject>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<TilemapObject>("TilemapObject")
			//.annotate(createEnTTComponentAttribute<TilemapObject>())
			;
	}

}