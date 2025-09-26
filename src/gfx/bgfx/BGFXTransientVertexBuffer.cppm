module;

#include <bgfx/bgfx.h>

export module Gfx.BGFX.BGFXTransientIndexBuffer;

export namespace Gfx::BGFX {

	struct BGFXTransientIndexBuffer {
		bgfx::TransientIndexBuffer buffer{};
	};

} // namespace Gfx::BGFX
