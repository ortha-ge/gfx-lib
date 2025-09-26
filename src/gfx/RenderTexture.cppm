module;

#include <cstdint>

export module Gfx.RenderTexture;

export namespace Gfx {

	struct RenderTexture {
		uint16_t width{};
		uint16_t height{};
	};

} // namespace Gfx
