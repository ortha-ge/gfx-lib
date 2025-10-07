export module Ortha.Gfx.Reflection.FontObject;

export import Ortha.Gfx.FontObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;

namespace Ortha::Core {

	template <>
	void reflect<Gfx::FontObject>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<FontObject>("FontObject")
			.field<&FontObject::text>("text")
			//.annotate(createEnTTComponentAttribute<FontObject>())
			;
	}

}
