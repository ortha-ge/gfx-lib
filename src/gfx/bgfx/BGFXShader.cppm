module;

#include <bgfx/bgfx.h>

export module Ortha.Gfx.BGFX.BGFXShader;

export namespace Ortha::Gfx::BGFX {

	struct BGFXShader {
		bgfx::ShaderHandle handle = BGFX_INVALID_HANDLE;
	};

} // namespace Ortha::Gfx::BGFX
