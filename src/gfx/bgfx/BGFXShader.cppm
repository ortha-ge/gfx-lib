module;

#include <bgfx/bgfx.h>

export module Gfx.BGFX.BGFXShader;

export namespace Gfx::BGFX {

	struct BGFXShader {
		bgfx::ShaderHandle handle = BGFX_INVALID_HANDLE;
	};

} // namespace Gfx::BGFX
