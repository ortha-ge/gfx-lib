
export module Ortha.Gfx.Reflection.ImageDescriptor;

export import Ortha.Gfx.ImageDescriptor;

import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::ImageDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<ImageDescriptor>("Image")
			.field<&ImageDescriptor::imageFilePath>("imageFilePath");
	}

}