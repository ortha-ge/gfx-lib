
export module Ortha.Gfx.Reflection.ImageDescriptor;

export import Ortha.Gfx.ImageDescriptor;

import Ortha.Core.Reflect;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::ImageDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<ImageDescriptor>("Image")
			.field<&ImageDescriptor::imageFilePath>("imageFilePath")
			;
	}

}