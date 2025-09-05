module;

#include <vector>

#include <entt/entity/entity.hpp>

export module Gfx.ShaderProgramResource;

import Gfx.ShaderProgramDescriptor;

export namespace Gfx {

    struct ShaderProgramResource {
		entt::entity vertexShaderResource{};
		entt::entity fragmentShaderResource{};
		std::vector<ShaderUniformDescriptor> uniforms;
		ShaderVertexLayoutDescriptor vertexLayout{};
    };

} // Gfx
