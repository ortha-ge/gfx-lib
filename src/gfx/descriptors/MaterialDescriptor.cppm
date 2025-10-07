module;

#include <optional>
#include <string>

export module Ortha.Gfx.MaterialDescriptor;

import glm;

export namespace Ortha::Gfx {

	struct MaterialDescriptor {
		bool operator==(const MaterialDescriptor&) const = default;
		std::string shaderProgramFilePath{};
		std::string textureImageFilePath{};
		std::optional<glm::vec4> alphaColour{};
	};

} // namespace Ortha::Gfx
