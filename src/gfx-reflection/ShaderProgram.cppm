module;

#include <functional>
#include <utility>

export module Ortha.Gfx.Reflection.ShaderProgramDescriptor;

export import Ortha.Gfx.ShaderProgramDescriptor;

import Ortha.Core.Reflect;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::ShaderVertexLayoutAttributeId>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addEnum<ShaderVertexLayoutAttributeId>("ShaderVertexLayoutAttributeId")
			.constant<ShaderVertexLayoutAttributeId::Position>("Position")
			.constant<ShaderVertexLayoutAttributeId::TexCoord0>("TexCoord0")
			.constant<ShaderVertexLayoutAttributeId::Colour0>("Colour0")
			;
	}

	template<>
	void reflect<Gfx::ShaderVertexLayoutAttribute>(RTTI::ReflectionContext& reflectionContext) {
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
	void reflect<Gfx::ShaderVertexLayoutDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflect<ShaderVertexLayoutAttribute>(reflectionContext);

		reflectionContext.addClass<ShaderVertexLayoutDescriptor>("ShaderVertexLayout")
			.field<&ShaderVertexLayoutDescriptor::attributes>("attributes")
			;
	}

	template<>
	void reflect<Gfx::ShaderUniformType>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addEnum<ShaderUniformType>("ShaderUniformType")
			.constant<ShaderUniformType::Sampler>("Sampler")
			.constant<ShaderUniformType::Float4>("Float4")
			;
	}

	template<>
	void reflect<Gfx::ShaderUniformDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflect<ShaderUniformType>(reflectionContext);

		reflectionContext.addClass<ShaderUniformDescriptor>("ShaderUniform")
			.field<&ShaderUniformDescriptor::name>("name")
			.field<&ShaderUniformDescriptor::type>("type")
			;
	}

	template<>
	void reflect<Gfx::ShaderProgramDescriptor>(RTTI::ReflectionContext& reflectionContext) {
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

}
