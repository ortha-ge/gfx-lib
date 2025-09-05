module;

#include <functional>
#include <utility>

export module Gfx.Reflection.TextureCoordinates;

export import Gfx.TextureCoordinates;

import Core.ClassReflection;
import Core.ReflectionContext;
import Core.TypeId;

template<>
void Core::reflect<Gfx::TextureCoordinates>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<TextureCoordinates>("TextureCoordinates")
		.property("x0", &TextureCoordinates::x0)
		.property("y0", &TextureCoordinates::y0)
		.property("x1", &TextureCoordinates::x1)
		.property("y1", &TextureCoordinates::y1)
		.build();
} // Gfx
