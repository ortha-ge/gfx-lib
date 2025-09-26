module;

#include <bgfx/bgfx.h>

export module Gfx.BGFX.BGFXTexture;

export namespace Gfx::BGFX {

	struct BGFXTexture {
		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
	};

} // namespace Gfx::BGFX
