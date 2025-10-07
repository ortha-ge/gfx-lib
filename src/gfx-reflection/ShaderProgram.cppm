module;

#include <functional>
#include <utility>

export module Gfx.Reflection.ShaderProgramDescriptor;

export import Gfx.ShaderProgramDescriptor;

import Core.Reflect;

template<>
void Core::reflect<Gfx::ShaderVertexLayoutAttributeId>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addEnum<ShaderVertexLayoutAttributeId>("ShaderVertexLayoutAttributeId")
		.constant<ShaderVertexLayoutAttributeId::Position>("Position")
		.constant<ShaderVertexLayoutAttributeId::TexCoord0>("TexCoord0")
		.constant<ShaderVertexLayoutAttributeId::Colour0>("Colour0")
		;
}

template<>
void Core::reflect<Gfx::ShaderVertexLayoutAttribute>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflect<ShaderVertexLayoutAttributeId>(reflectionContext);

	reflectionContext.addClass<ShaderVertexLayoutAttribute>("ShaderVertexLayoutAttribute")
		.field<&ShaderVertexLayoutAttribute::attributeId>("id")
		.field<&ShaderVertexLayoutAttribute::count>("elementCount")
		.field<&ShaderVertexLayoutAttribute::typeId>("elementType")
		.field<&ShaderVertexLayoutAttribute::isNormalized>("isNormalized")
		;
}

template<>
void Core::reflect<Gfx::ShaderVertexLayoutDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflect<ShaderVertexLayoutAttribute>(reflectionContext);

	reflectionContext.addClass<ShaderVertexLayoutDescriptor>("ShaderVertexLayout")
		.field<&ShaderVertexLayoutDescriptor::attributes>("attributes")
		;
}

template<>
void Core::reflect<Gfx::ShaderUniformType>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addEnum<ShaderUniformType>("ShaderUniformType")
		.constant<ShaderUniformType::Sampler>("Sampler")
		.constant<ShaderUniformType::Float4>("Float4")
		;
}

template<>
void Core::reflect<Gfx::ShaderUniformDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflect<ShaderUniformType>(reflectionContext);

	reflectionContext.addClass<ShaderUniformDescriptor>("ShaderUniform")
		.field<&ShaderUniformDescriptor::name>("name")
		.field<&ShaderUniformDescriptor::type>("type")
		;
}

template<>
void Core::reflect<Gfx::ShaderProgramDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflect<ShaderUniformDescriptor>(reflectionContext);
	reflect<ShaderVertexLayoutDescriptor>(reflectionContext);

	reflectionContext.addClass<ShaderProgramDescriptor>("ShaderProgram")
		.field<&ShaderProgramDescriptor::vertexShaderFilePath>("vertexShaderFilePath")
		.field<&ShaderProgramDescriptor::fragmentShaderFilePath>("fragmentShaderFilePath")
		.field<&ShaderProgramDescriptor::uniforms>("uniforms")
		.field<&ShaderProgramDescriptor::vertexLayout>("vertexLayout")
		;
}
