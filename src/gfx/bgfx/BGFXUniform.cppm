module;

#include <string>
#include <unordered_map>

#include <bgfx/bgfx.h>

export module Gfx.BGFX.BGFXUniform;

export namespace Gfx::BGFX {

	struct BGFXUniform {
		bgfx::UniformHandle handle = BGFX_INVALID_HANDLE;
	};

	struct BGFXUniforms {
		std::unordered_map<std::string, bgfx::UniformHandle> uniforms{};
	};

} // namespace Gfx::BGFX
