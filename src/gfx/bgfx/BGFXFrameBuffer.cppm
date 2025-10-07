module;

#include <bgfx/bgfx.h>

export module Ortha.Gfx.BGFX.BGFXFrameBuffer;

export namespace Ortha::Gfx::BGFX {

	struct BGFXFrameBuffer {
		bgfx::FrameBufferHandle handle = BGFX_INVALID_HANDLE;
	};

} // namespace Ortha::Gfx::BGFX
