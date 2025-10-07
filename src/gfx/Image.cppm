module;

#include <cstdint>
#include <vector>

export module Ortha.Gfx.Image;

export namespace Ortha::Gfx {

	struct Image {
		std::vector<uint8_t> image{};
		uint32_t format{};
		uint32_t size{};
		uint32_t offset{};
		uint32_t width{};
		uint32_t height{};
		uint32_t depth{};
		uint16_t numLayers{};
		uint8_t numMips{};
		bool hasAlpha{};
		bool cubeMap{};
		bool ktx{};
		bool ktxLE{};
		bool pvr3{};
		bool srgb{};
	};

} // namespace Ortha::Gfx
