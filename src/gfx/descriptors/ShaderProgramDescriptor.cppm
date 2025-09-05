module;

#include <cstdint>
#include <string>
#include <vector>

export module Gfx.ShaderProgramDescriptor;

import Core.TypeId;

export namespace Gfx {

	// Vertex Layout
	// Uniform Descriptors

	enum class ShaderVertexLayoutAttributeId : uint8_t { Position, TexCoord0 };

	struct ShaderVertexLayoutAttribute {
		ShaderVertexLayoutAttributeId attributeId{};
		Core::TypeId typeId;
		uint8_t count{};
	};

	struct ShaderVertexLayoutDescriptor {
		std::vector<ShaderVertexLayoutAttribute> attributes;
	};

	enum class ShaderUniformType : uint8_t { Sampler, Float4 };

	struct ShaderUniformDescriptor {
		ShaderUniformType type{};
		std::string name;
	};

	struct ShaderProgramDescriptor {
		std::string vertexShaderFilePath;
		std::string fragmentShaderFilePath;
		std::vector<ShaderUniformDescriptor> uniforms;
		ShaderVertexLayoutDescriptor vertexLayout{};
	};

} // namespace Gfx
