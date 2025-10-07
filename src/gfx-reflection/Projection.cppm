module;

#include <cstdio>
#include <type_traits>

export module Ortha.Gfx.Reflection.Projection;

export import Ortha.Gfx.Projection;

import Ortha.Core.Reflect;

template<>
void Core::reflect<Ortha::Gfx::Projection>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;

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
