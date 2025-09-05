export module Gfx.Reflection.Projection;

export import Gfx.Projection;

import Core.ReflectionContext;

template <>
void Core::reflect<Gfx::Projection>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addEnum<Projection>("Projection")
		.constant("Orthographic", Projection::Orthographic)
		.constant("Perspective", Projection::Perspective)
	.build();
}