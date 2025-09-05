module;

#include <functional>
#include <utility>

export module Gfx.Reflection.Colour;

export import Gfx.Colour;

import Core.ClassReflection;
import Core.ReflectionContext;
import Core.TypeId;

template<>
void Core::reflect<Gfx::Colour>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<Colour>("Colour")
		.property("r", &Colour::r)
		.property("g", &Colour::g)
		.property("b", &Colour::b)
		.property("a", &Colour::a)
		.build();
} // Gfx
