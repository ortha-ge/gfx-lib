
export module Gfx.Reflection.ImageDescriptor;

export import Gfx.ImageDescriptor;

import Core.ClassReflection;

import Core.ReflectionContext;
import Core.TypeId;

template<>
void Core::reflect<Gfx::ImageDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<ImageDescriptor>("Image")
		.property("imageFilePath", &ImageDescriptor::imageFilePath)
		.build();
}
