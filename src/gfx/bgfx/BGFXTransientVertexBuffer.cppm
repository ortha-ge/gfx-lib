module;

#include <bgfx/bgfx.h>

export module Ortha.Gfx.BGFX.BGFXTransientIndexBuffer;

export namespace Ortha::Gfx::BGFX {

	struct BGFXTransientIndexBuffer {
		bgfx::TransientIndexBuffer buffer{};
	};

} // namespace Ortha::Gfx::BGFX
