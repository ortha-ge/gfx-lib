module;

#include <optional>
#include <vector>

#include <entt/entity/entity.hpp>

export module Gfx.Material;

import Gfx.Colour;
import Gfx.TextureCoordinates;

export namespace Gfx {

	struct Material {
		entt::entity shaderProgram{ entt::null };
		entt::entity textureImage{ entt::null };
		std::vector<TextureCoordinates> spriteFrames{};
		std::optional<Colour> alphaColour{};
		float width{};
		float height{};
	};

} // namespace Gfx
