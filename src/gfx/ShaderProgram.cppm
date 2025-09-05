module;

#include <vector>

#include <entt/entity/entity.hpp>

export module Gfx.ShaderProgram;

import Gfx.ShaderProgramDescriptor;

export namespace Gfx {

	struct ShaderProgram {
		entt::entity vertexShader{};
		entt::entity fragmentShader{};
		std::vector<ShaderUniformDescriptor> uniforms;
		ShaderVertexLayoutDescriptor vertexLayout{};
	};

} // namespace Gfx
