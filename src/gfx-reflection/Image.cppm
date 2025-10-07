
export module Ortha.Gfx.Reflection.ImageDescriptor;

export import Ortha.Gfx.ImageDescriptor;

import Ortha.Core.Reflect;

template<>
void Core::reflect<Ortha::Gfx::ImageDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<ImageDescriptor>("Image")
		.field<&ImageDescriptor::imageFilePath>("imageFilePath")
		;
}
