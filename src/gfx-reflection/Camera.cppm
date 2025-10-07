export module Gfx.Reflection.Camera;

export import Gfx.Camera;
export import Gfx.CameraDescriptor;

import Core.EnTTComponentAttribute;
import Core.Reflect;
import Gfx.Reflection.Projection;

template<>
void Core::reflect<Gfx::CameraDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<CameraDescriptor>("CameraDescriptor")
		.field<&CameraDescriptor::projection>("projection")
		;
}

template <>
void Core::reflect<Gfx::Camera>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<Camera>("Camera")
		//.annotate(createEnTTComponentAttribute<Camera>())
		;
}