module;

#include <optional>
#include <string>

#include <glm/vec4.hpp>

export module Gfx.MaterialDescriptor;

export namespace Gfx {

	struct MaterialDescriptor {
		bool operator==(const MaterialDescriptor&) const = default;
		std::string shaderProgramFilePath{};
		std::string textureImageFilePath{};
		std::optional<glm::vec4> alphaColour{};
	};

} // namespace Gfx
