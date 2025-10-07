export module Ortha.Gfx.Reflection.Camera;

export import Ortha.Gfx.Camera;
export import Ortha.Gfx.CameraDescriptor;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;
import Ortha.Gfx.Reflection.Projection;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::CameraDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<CameraDescriptor>("CameraDescriptor")
			.field<&CameraDescriptor::projection>("projection")
			;
	}

	template <>
	void reflect<Gfx::Camera>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<Camera>("Camera")
			//.annotate(createEnTTComponentAttribute<Camera>())
			;
	}

}