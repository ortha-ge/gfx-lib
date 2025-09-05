export module Gfx.Reflection.Camera;

export import Gfx.CameraDescriptor;

import Core.ClassReflection;
import Core.ReflectionContext;
import Gfx.Reflection.Projection;

template <>
void Core::reflect<Gfx::CameraDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<CameraDescriptor>("Camera")
		.property("projection", &CameraDescriptor::projection)
	.build();
}