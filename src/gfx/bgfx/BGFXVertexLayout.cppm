module;

#include <bgfx/bgfx.h>

export module Ortha.Gfx.BGFX.BGFXVertexLayout;

export namespace Ortha::Gfx::BGFX {

	struct BGFXVertexLayout {
		//bgfx::VertexLayoutHandle handle = BGFX_INVALID_HANDLE;
		bgfx::VertexLayout layout;
	};

} // namespace Ortha::Gfx::BGFX
