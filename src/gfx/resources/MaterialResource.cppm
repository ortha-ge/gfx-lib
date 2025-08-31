module;

#include <optional>
#include <vector>

#include <entt/entity/entity.hpp>

export module Gfx.MaterialResource;

import Gfx.Colour;
import Gfx.TextureCoordinates;

export namespace Gfx {

    struct MaterialResource {
        entt::entity shaderProgramResource{ entt::null };
        entt::entity textureImageResource{ entt::null };
        std::vector<TextureCoordinates> spriteFrames{};
        std::optional<Colour> alphaColour{};
        float width{};
        float height{};
    };

} // Gfx
