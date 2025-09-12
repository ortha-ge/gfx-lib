module;

#include <optional>

#include <entt/entity/entity.hpp>
#include <glm/vec4.hpp>

export module Gfx.Material;

import Core.ResourceHandle;
import Gfx.TextureCoordinates;

export namespace Gfx {

	struct Material {
		std::shared_ptr<Core::ResourceHandle> shaderProgram{};
		std::shared_ptr<Core::ResourceHandle> textureImage{};
		std::optional<glm::vec4> alphaColour{};
	};

} // namespace Gfx
