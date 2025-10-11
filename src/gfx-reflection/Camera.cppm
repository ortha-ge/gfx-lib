export module Ortha.Gfx.Reflection.Camera;

export import Ortha.Gfx.Camera;
export import Ortha.Gfx.CameraDescriptor;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Gfx.Reflection.Projection;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::CameraDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<CameraDescriptor>("CameraDescriptor")
			.field<&CameraDescriptor::projection>("projection")
			;
	}

	template <>
	void reflect<Gfx::Camera>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<Camera>("Camera")
			//.annotate(createEnTTComponentAttribute<Camera>())
			;
	}

}