module;

#include <bgfx/bgfx.h>

export module Gfx.BGFX.BGFXShaderProgram;

export namespace Gfx::BGFX {

	struct BGFXShaderProgram {
		bgfx::ProgramHandle programHandle = BGFX_INVALID_HANDLE;
	};

} // namespace Gfx::BGFX
