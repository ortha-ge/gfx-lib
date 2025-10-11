module;

#include <cstdio>
#include <type_traits>

export module Ortha.Gfx.Reflection.Projection;

export import Ortha.Gfx.Projection;

import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::Projection>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addEnum<Projection>("Projection")
			.constant<Projection::Orthographic>("Orthographic")
			.constant<Projection::Perspective>("Perspective")
			;
	}

}
