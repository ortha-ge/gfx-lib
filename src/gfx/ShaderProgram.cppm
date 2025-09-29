module;

#include <memory>
#include <vector>

export module Gfx.ShaderProgram;

import Gfx.ShaderProgramDescriptor;

import Core.ResourceHandle;

export namespace Gfx {

	struct ShaderProgram {
		std::shared_ptr<Core::ResourceHandle> vertexShader{};
		std::shared_ptr<Core::ResourceHandle> fragmentShader{};
		std::vector<ShaderUniformDescriptor> uniforms;
		ShaderVertexLayoutDescriptor vertexLayout{};
	};

} // namespace Gfx
