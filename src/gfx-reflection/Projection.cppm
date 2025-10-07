module;

#include <cstdio>
#include <type_traits>

export module Ortha.Gfx.Reflection.Projection;

export import Ortha.Gfx.Projection;

import Ortha.Core.Reflect;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::Projection>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addEnum<Projection>("Projection")
			.constant<Projection::Orthographic>("Orthographic")
			.constant<Projection::Perspective>("Perspective")
			;
	}

}
