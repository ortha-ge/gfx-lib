export module Ortha.Gfx.Reflection.Camera;

export import Ortha.Gfx.Camera;
export import Ortha.Gfx.CameraDescriptor;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;
import Ortha.Gfx.Reflection.Projection;

template<>
void Core::reflect<Ortha::Gfx::CameraDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<CameraDescriptor>("CameraDescriptor")
		.field<&CameraDescriptor::projection>("projection")
		;
}

template <>
void Core::reflect<Ortha::Gfx::Camera>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<Camera>("Camera")
		//.annotate(createEnTTComponentAttribute<Camera>())
		;
}