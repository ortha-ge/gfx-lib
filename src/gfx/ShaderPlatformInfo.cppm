module;

#include <optional>
#include <string>

export module Gfx.ShaderPlatformInfo;

export namespace Gfx {

	struct ShaderPlatformInfo {
		std::optional<std::string> shadersFolderName;
	};

} // namespace Gfx