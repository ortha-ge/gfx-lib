module;

#include <optional>
#include <string>

export module Ortha.Gfx.ShaderPlatformInfo;

export namespace Ortha::Gfx {

	struct ShaderPlatformInfo {
		std::optional<std::string> shadersFolderName;
	};

} // namespace Ortha::Gfx