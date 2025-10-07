module;

#include <cstdio>
#include <type_traits>

export module Gfx.Reflection.Projection;

export import Gfx.Projection;

import Core.Reflect;

template<>
void Core::reflect<Gfx::Projection>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	if constexpr (requires(Projection a, Projection b) { a = b; }) {
		printf("Yup\n");
	} else {
		printf("Nope\n");
	}

	reflectionContext.addEnum<Projection>("Projection")
		.constant<Projection::Orthographic>("Orthographic")
		.constant<Projection::Perspective>("Perspective")
		;
}
