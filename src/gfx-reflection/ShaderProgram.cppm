module;

#include <functional>
#include <utility>

export module Gfx.Reflection.ShaderProgramDescriptor;

export import Gfx.ShaderProgramDescriptor;

import Core.ClassReflection;
import Core.EnumReflection;
import Core.ReflectionContext;
import Core.TypeId;

template<>
void Core::reflect<Gfx::ShaderVertexLayoutAttributeId>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addEnum<ShaderVertexLayoutAttributeId>("ShaderVertexLayoutAttributeId")
		.constant("Position", ShaderVertexLayoutAttributeId::Position)
		.constant("TexCoord0", ShaderVertexLayoutAttributeId::TexCoord0)
		.build();
}

template<>
void Core::reflect<Gfx::ShaderVertexLayoutAttribute>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<ShaderVertexLayoutAttribute>("ShaderVertexLayoutAttribute")
		.property("id", &ShaderVertexLayoutAttribute::attributeId)
		.property("elementCount", &ShaderVertexLayoutAttribute::count)
		.property("elementType", &ShaderVertexLayoutAttribute::typeId)
		.build();
}

template<>
void Core::reflect<Gfx::ShaderVertexLayoutDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<ShaderVertexLayoutDescriptor>("ShaderVertexLayout")
		.property("attributes", &ShaderVertexLayoutDescriptor::attributes)
		.build();
}

template<>
void Core::reflect<Gfx::ShaderUniformType>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addEnum<ShaderUniformType>("ShaderUniformType")
		.constant("Sampler", ShaderUniformType::Sampler)
		.constant("Float4", ShaderUniformType::Float4)
		.build();
}

template<>
void Core::reflect<Gfx::ShaderUniformDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<ShaderUniformDescriptor>("ShaderUniform")
		.property("name", &ShaderUniformDescriptor::name)
		.property("type", &ShaderUniformDescriptor::type)
		.build();
}

template<>
void Core::reflect<Gfx::ShaderProgramDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<ShaderProgramDescriptor>("ShaderProgram")
		.property("vertexShaderFilePath", &ShaderProgramDescriptor::vertexShaderFilePath)
		.property("fragmentShaderFilePath", &ShaderProgramDescriptor::fragmentShaderFilePath)
		.property("uniforms", &ShaderProgramDescriptor::uniforms)
		.property("vertexLayout", &ShaderProgramDescriptor::vertexLayout)
		.build();
}
