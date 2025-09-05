module;

#include <functional>
#include <utility>

export module Gfx.Reflection.ShaderProgramDescriptor;

import Core.ClassReflection;
import Core.EnumReflection;
import Core.ReflectionContext;
import Core.TypeId;
export import Gfx.ShaderProgramDescriptor;

template <>
void Core::reflect<Gfx::ShaderVertexLayoutAttributeId>(ReflectionContext& context) {
	using namespace Gfx;
	auto enumReflection = EnumReflectionBuilder<ShaderVertexLayoutAttributeId>("ShaderVertexLayoutAttributeId")
		.constant("Position", ShaderVertexLayoutAttributeId::Position)
		.constant("TexCoord0", ShaderVertexLayoutAttributeId::TexCoord0)
		.build();

	context.addEnum(TypeId::get<ShaderVertexLayoutAttributeId>(), std::move(enumReflection));
}

template <>
void Core::reflect<Gfx::ShaderVertexLayoutAttribute>(ReflectionContext& context) {
	using namespace Gfx;
	reflect<ShaderVertexLayoutAttributeId>(context);

	auto classReflection = ClassReflectionBuilder<ShaderVertexLayoutAttribute>("ShaderVertexLayoutAttribute")
		.property("id", &ShaderVertexLayoutAttribute::attributeId)
		.property("elementCount", &ShaderVertexLayoutAttribute::count)
		.property("elementType", &ShaderVertexLayoutAttribute::typeId)
		.build();

	context.addClass(TypeId::get<ShaderVertexLayoutAttribute>(), std::move(classReflection));
}

template <>
void Core::reflect<Gfx::ShaderVertexLayoutDescriptor>(ReflectionContext& context) {
	using namespace Gfx;
	auto classReflection = ClassReflectionBuilder<ShaderVertexLayoutDescriptor>("ShaderVertexLayout")
		.property("attributes", &ShaderVertexLayoutDescriptor::attributes)
		.build();

	context.addClass(TypeId::get<ShaderVertexLayoutDescriptor>(), std::move(classReflection));
}

template <>
void Core::reflect<Gfx::ShaderUniformType>(ReflectionContext& context) {
	using namespace Gfx;
	auto enumReflection = EnumReflectionBuilder<ShaderUniformType>("ShaderUniformType")
		.constant("Sampler", ShaderUniformType::Sampler)
		.constant("Float4", ShaderUniformType::Float4)
		.build();

	context.addEnum(TypeId::get<ShaderUniformType>(), std::move(enumReflection));
}

template <>
void Core::reflect<Gfx::ShaderUniformDescriptor>(ReflectionContext& context) {
	Core::reflect<Gfx::ShaderUniformType>(context);

	using namespace Gfx;
	auto classReflection = ClassReflectionBuilder<ShaderUniformDescriptor>("ShaderUniform")
		.property("name", &ShaderUniformDescriptor::name)
		.property("type", &ShaderUniformDescriptor::type)
		.build();

	context.addClass(TypeId::get<ShaderUniformDescriptor>(), std::move(classReflection));
}

template <>
void Core::reflect<Gfx::ShaderProgramDescriptor>(ReflectionContext& context) {
	reflect<Gfx::ShaderVertexLayoutAttribute>(context);
	reflect<Gfx::ShaderVertexLayoutDescriptor>(context);
	reflect<Gfx::ShaderUniformDescriptor>(context);

    using namespace Gfx;
    auto classReflection = ClassReflectionBuilder<ShaderProgramDescriptor>("ShaderProgram")
        .property("vertexShaderFilePath", &ShaderProgramDescriptor::vertexShaderFilePath)
        .property("fragmentShaderFilePath", &ShaderProgramDescriptor::fragmentShaderFilePath)
		.property("uniforms", &ShaderProgramDescriptor::uniforms)
		.property("vertexLayout", &ShaderProgramDescriptor::vertexLayout)
        .build();

    context.addClass(TypeId::get<ShaderProgramDescriptor>(), std::move(classReflection));
}
