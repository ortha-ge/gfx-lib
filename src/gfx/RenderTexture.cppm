module;

#include <cstdint>

export module Ortha.Gfx.RenderTexture;

export namespace Ortha::Gfx {

	struct RenderTexture {
		uint16_t width{};
		uint16_t height{};
	};

} // namespace Ortha::Gfx
