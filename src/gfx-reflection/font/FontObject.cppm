export module Ortha.Gfx.Reflection.FontObject;

export import Ortha.Gfx.FontObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;

template <>
void Core::reflect<Ortha::Gfx::FontObject>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;

	reflectionContext.addClass<FontObject>("FontObject")
		.field<&FontObject::text>("text")
		//.annotate(createEnTTComponentAttribute<FontObject>())
		;
}