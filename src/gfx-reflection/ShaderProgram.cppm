module;

#include <functional>
#include <utility>

export module Ortha.Gfx.Reflection.ShaderProgramDescriptor;

export import Ortha.Gfx.ShaderProgramDescriptor;

import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::ShaderVertexLayoutAttributeId>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addEnum<ShaderVertexLayoutAttributeId>("ShaderVertexLayoutAttributeId")
			.constant<ShaderVertexLayoutAttributeId::Position>("Position")
			.constant<ShaderVertexLayoutAttributeId::TexCoord0>("TexCoord0")
			.constant<ShaderVertexLayoutAttributeId::Colour0>("Colour0");
	}

	template<>
	void reflect<Gfx::ShaderVertexLayoutAttribute>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		ensureReflected<ShaderVertexLayoutAttributeId>(reflectionContext);
		reflectionContext.addClass<ShaderVertexLayoutAttribute>("ShaderVertexLayoutAttribute")
			.field<&ShaderVertexLayoutAttribute::attributeId>("id")
			.field<&ShaderVertexLayoutAttribute::count>("elementCount")
			.field<&ShaderVertexLayoutAttribute::typeId>("elementType")
			.field<&ShaderVertexLayoutAttribute::isNormalized>("isNormalized");
	}

	template<>
	void reflect<Gfx::ShaderVertexLayoutDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		ensureReflected<ShaderVertexLayoutAttribute>(reflectionContext);
		reflectionContext.addClass<ShaderVertexLayoutDescriptor>("ShaderVertexLayout")
			.field<&ShaderVertexLayoutDescriptor::attributes>("attributes");
	}

	template<>
	void reflect<Gfx::ShaderUniformType>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addEnum<ShaderUniformType>("ShaderUniformType")
			.constant<ShaderUniformType::Sampler>("Sampler")
			.constant<ShaderUniformType::Float4>("Float4");
	}

	template<>
	void reflect<Gfx::ShaderUniformDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		ensureReflected<ShaderUniformType>(reflectionContext);
		reflectionContext.addClass<ShaderUniformDescriptor>("ShaderUniform")
			.field<&ShaderUniformDescriptor::name>("name")
			.field<&ShaderUniformDescriptor::type>("type");
	}

	template<>
	void reflect<Gfx::ShaderProgramDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		ensureReflected<ShaderUniformDescriptor>(reflectionContext);
		ensureReflected<ShaderVertexLayoutDescriptor>(reflectionContext);
		reflectionContext.addClass<ShaderProgramDescriptor>("ShaderProgram")
			.field<&ShaderProgramDescriptor::vertexShaderFilePath>("vertexShaderFilePath")
			.field<&ShaderProgramDescriptor::fragmentShaderFilePath>("fragmentShaderFilePath")
			.field<&ShaderProgramDescriptor::uniforms>("uniforms")
			.field<&ShaderProgramDescriptor::vertexLayout>("vertexLayout");
	}

}
