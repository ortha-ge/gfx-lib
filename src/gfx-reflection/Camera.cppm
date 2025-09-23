export module Gfx.Reflection.Camera;

export import Gfx.Camera;
export import Gfx.CameraDescriptor;

import Core.EnTTComponentAttribute;
import Core.ClassReflection;
import Core.ReflectionContext;
import Gfx.Reflection.Projection;

template<>
void Core::reflect<Gfx::CameraDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<CameraDescriptor>("CameraDescriptor")
		.property("projection", &CameraDescriptor::projection)
		.build();
}

template <>
void Core::reflect<Gfx::Camera>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<Camera>("Camera")
		.annotate(createEnTTComponentAttribute<Camera>())
		.build();
}