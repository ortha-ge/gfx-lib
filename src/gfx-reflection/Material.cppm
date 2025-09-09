module;

#include <functional>
#include <utility>

export module Gfx.Reflection.MaterialDescriptor;

export import Gfx.MaterialDescriptor;

import Core.ClassReflection;
import Core.ReflectionContext;
import Core.TypeId;
import Gfx.Reflection.Colour;

template<>
void Core::reflect<Gfx::MaterialDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<MaterialDescriptor>("Material")
		.property("shaderProgramFilePath", &MaterialDescriptor::shaderProgramFilePath)
		.property("textureImageFilePath", &MaterialDescriptor::textureImageFilePath)
		.property("width", &MaterialDescriptor::width)
		.property("height", &MaterialDescriptor::height)
		.property("alphaColour", &MaterialDescriptor::alphaColour)
		.build();
}
