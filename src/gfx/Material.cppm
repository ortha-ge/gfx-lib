module;

#include <memory>
#include <optional>

export module Gfx.Material;

import Core.ResourceHandle;
import Gfx.TextureCoordinates;
import glm;

export namespace Gfx {

	struct Material {
		std::shared_ptr<Core::ResourceHandle> shaderProgram{};
		std::shared_ptr<Core::ResourceHandle> textureImage{};
		std::optional<glm::vec4> alphaColour{};
	};

} // namespace Gfx
