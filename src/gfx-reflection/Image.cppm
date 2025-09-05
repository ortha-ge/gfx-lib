module;

#include <utility>

export module Core.ImageDescriptor.Reflection;

import Core.ClassReflection;
import Core.ImageDescriptor;
import Core.ReflectionContext;
import Core.TypeId;

template <>
void Core::reflect<Core::ImageDescriptor>(ReflectionContext& reflectionContext) {
	reflectionContext.addClass<ImageDescriptor>("Image")
		.property("imageFilePath", &ImageDescriptor::imageFilePath)
	.build();
}