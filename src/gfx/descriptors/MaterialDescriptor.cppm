module;

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

export module Gfx.MaterialDescriptor;

import Gfx.Colour;

export namespace Gfx {

	struct MaterialDescriptor {
		std::string shaderProgramFilePath{};
		std::string textureImageFilePath{};
		std::optional<Colour> alphaColour{};
		float width{};
		float height{};
	};

} // namespace Gfx
