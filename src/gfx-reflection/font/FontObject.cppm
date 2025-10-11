export module Ortha.Gfx.Reflection.FontObject;

export import Ortha.Gfx.FontObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template <>
	void reflect<Gfx::FontObject>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<FontObject>("FontObject")
			.field<&FontObject::text>("text");
			//.annotate(createEnTTComponentAttribute<FontObject>())
	}

}
