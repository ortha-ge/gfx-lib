module;

#include <entt/entity/entity.hpp>

export module Gfx.ShaderProgramResource;

export namespace Gfx {

    struct ShaderProgramResource {
        entt::entity vertexShaderResource{};
        entt::entity fragmentShaderResource{};
    };

} // Gfx
