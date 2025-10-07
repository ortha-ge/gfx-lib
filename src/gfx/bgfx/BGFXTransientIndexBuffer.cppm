module;

#include <bgfx/bgfx.h>

export module Ortha.Gfx.BGFX.BGFXTransientVertexBuffer;

export namespace Ortha::Gfx::BGFX {

	struct BGFXTransientVertexBuffer {
		bgfx::TransientVertexBuffer buffer{};
	};

} // namespace Ortha::Gfx::BGFX
