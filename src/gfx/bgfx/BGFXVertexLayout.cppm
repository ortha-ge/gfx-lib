module;

#include <bgfx/bgfx.h>

export module Gfx.BGFX.BGFXVertexLayout;

export namespace Gfx::BGFX {

	struct BGFXVertexLayout {
		//bgfx::VertexLayoutHandle handle = BGFX_INVALID_HANDLE;
		bgfx::VertexLayout layout;
	};

} // namespace Gfx::BGFX
