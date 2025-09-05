module;

#include <functional>
#include <utility>

export module Gfx.Reflection.MaterialDescriptor;

import Core.ClassReflection;
import Core.ReflectionContext;
import Core.TypeId;
import Gfx.MaterialDescriptor;
import Gfx.Reflection.Colour;
import Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Gfx::MaterialDescriptor>(ReflectionContext& reflectionContext) {
    using namespace Gfx;
	reflect<TextureCoordinates>(reflectionContext);
	reflect<Colour>(reflectionContext);

    reflectionContext.addClass<MaterialDescriptor>("Material")
        .property("shaderProgramFilePath", &MaterialDescriptor::shaderProgramFilePath)
        .property("textureImageFilePath", &MaterialDescriptor::textureImageFilePath)
        .property("width", &MaterialDescriptor::width)
        .property("height", &MaterialDescriptor::height)
        .property("spriteFrames", &MaterialDescriptor::spriteFrames)
        .property("alphaColour", &MaterialDescriptor::alphaColour)
    .build();
}
