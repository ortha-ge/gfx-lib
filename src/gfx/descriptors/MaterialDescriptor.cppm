module;

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

export module Gfx.MaterialDescriptor;

import Gfx.Colour;
import Gfx.TextureCoordinates;

export namespace Gfx {

	struct MaterialDescriptor {
		std::string shaderProgramFilePath{};
		std::string textureImageFilePath{};
		std::vector<TextureCoordinates> spriteFrames{};
		std::optional<Colour> alphaColour{};
		uint16_t renderPass{ 0u };
		float width{};
		float height{};
	};

} // namespace Gfx
