module;

#include <functional>
#include <utility>

export module Gfx.Reflection.MaterialDescriptor;

import Core.ClassReflection;
import Core.ReflectionContext;
import Core.TypeId;
export import Gfx.MaterialDescriptor;

template<>
void Core::reflect<Gfx::MaterialDescriptor>(ReflectionContext& context) {
    using namespace Gfx;
    auto classReflection = ClassReflectionBuilder<MaterialDescriptor>("Material")
        .property("shaderProgramFilePath", &MaterialDescriptor::shaderProgramFilePath)
        .property("textureImageFilePath", &MaterialDescriptor::textureImageFilePath)
        .property("width", &MaterialDescriptor::width)
        .property("height", &MaterialDescriptor::height)
        .property("spriteFrames", &MaterialDescriptor::spriteFrames)
        .property("alphaColour", &MaterialDescriptor::alphaColour)
        .build();

    context.addClass(TypeId::get<MaterialDescriptor>(), std::move(classReflection));
}
