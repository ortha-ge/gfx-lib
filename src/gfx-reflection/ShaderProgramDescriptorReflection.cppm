module;

#include <functional>
#include <utility>

export module Gfx.Reflection.ShaderProgramDescriptor;

import Core.ClassReflection;
import Core.ReflectionContext;
import Core.TypeId;
export import Gfx.ShaderProgramDescriptor;

template <>
void Core::reflect<Gfx::ShaderProgramDescriptor>(ReflectionContext& context) {
    using namespace Gfx;
    auto classReflection = ClassReflectionBuilder<ShaderProgramDescriptor>("ShaderProgram")
        .property("vertexShaderFilePath", &ShaderProgramDescriptor::vertexShaderFilePath)
        .property("fragmentShaderFilePath", &ShaderProgramDescriptor::fragmentShaderFilePath)
        .build();

    context.addClass(TypeId::get<ShaderProgramDescriptor>(), std::move(classReflection));
}
