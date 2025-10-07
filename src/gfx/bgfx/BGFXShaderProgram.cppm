module;

#include <bgfx/bgfx.h>

export module Ortha.Gfx.BGFX.BGFXShaderProgram;

export namespace Ortha::Gfx::BGFX {

	struct BGFXShaderProgram {
		bgfx::ProgramHandle programHandle = BGFX_INVALID_HANDLE;
	};

} // namespace Ortha::Gfx::BGFX
