module;

#include <bgfx/bgfx.h>

export module Ortha.Gfx.BGFX.BGFXTexture;

export namespace Ortha::Gfx::BGFX {

	struct BGFXTexture {
		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
	};

} // namespace Ortha::Gfx::BGFX
