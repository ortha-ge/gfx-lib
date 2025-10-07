module;

#include <memory>
#include <vector>

export module Ortha.Gfx.ShaderProgram;

import Ortha.Gfx.ShaderProgramDescriptor;

import Ortha.Core.ResourceHandle;

export namespace Ortha::Gfx {

	struct ShaderProgram {
		std::shared_ptr<Core::ResourceHandle> vertexShader{};
		std::shared_ptr<Core::ResourceHandle> fragmentShader{};
		std::vector<ShaderUniformDescriptor> uniforms;
		ShaderVertexLayoutDescriptor vertexLayout{};
	};

} // namespace Ortha::Gfx
