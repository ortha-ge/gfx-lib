module;

#include <optional>
#include <string>

#include <glm/vec4.hpp>

export module Gfx.MaterialDescriptor;

export namespace Gfx {

	struct MaterialDescriptor {
		std::string shaderProgramFilePath{};
		std::string textureImageFilePath{};
		std::optional<glm::vec4> alphaColour{};
	};

} // namespace Gfx
