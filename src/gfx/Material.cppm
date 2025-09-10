module;

#include <optional>

#include <entt/entity/entity.hpp>

export module Gfx.Material;

import Core.ResourceHandle;
import Gfx.Colour;
import Gfx.TextureCoordinates;

export namespace Gfx {

	struct Material {
		std::shared_ptr<Core::ResourceHandle> shaderProgram{};
		std::shared_ptr<Core::ResourceHandle> textureImage{};
		std::optional<Colour> alphaColour{};
		float width{};
		float height{};
	};

} // namespace Gfx
