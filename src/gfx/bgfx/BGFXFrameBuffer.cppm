module;

#include <bgfx/bgfx.h>

export module Gfx.BGFX.BGFXFrameBuffer;

export namespace Gfx::BGFX {

	struct BGFXFrameBuffer {
		bgfx::FrameBufferHandle handle = BGFX_INVALID_HANDLE;
	};

} // namespace Gfx::BGFX
