
export module Gfx.Reflection.ImageDescriptor;

export import Gfx.ImageDescriptor;

import Core.Reflect;

template<>
void Core::reflect<Gfx::ImageDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<ImageDescriptor>("Image")
		.field<&ImageDescriptor::imageFilePath>("imageFilePath")
		;
}
