module;

#include <bgfx/bgfx.h>

export module Gfx.BGFX.BGFXTransientVertexBuffer;

export namespace Gfx::BGFX {

	struct BGFXTransientVertexBuffer {
		bgfx::TransientVertexBuffer buffer{};
	};

} // namespace Gfx::BGFX
