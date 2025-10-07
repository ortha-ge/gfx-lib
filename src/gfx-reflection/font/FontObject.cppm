export module Gfx.Reflection.FontObject;

export import Gfx.FontObject;

import Core.EnTTComponentAttribute;
import Core.Reflect;

template <>
void Core::reflect<Gfx::FontObject>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<FontObject>("FontObject")
		.field<&FontObject::text>("text")
		//.annotate(createEnTTComponentAttribute<FontObject>())
		;
}