module;

#include <memory>
#include <optional>

export module Ortha.Gfx.Material;

import Ortha.Core.ResourceHandle;
import Ortha.Gfx.TextureCoordinates;
import glm;

export namespace Ortha::Gfx {

	struct Material {
		std::shared_ptr<Core::ResourceHandle> shaderProgram{};
		std::shared_ptr<Core::ResourceHandle> textureImage{};
		std::optional<glm::vec4> alphaColour{};
	};

} // namespace Ortha::Gfx
