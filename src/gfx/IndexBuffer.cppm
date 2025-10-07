module;

#include <cstdint>
#include <vector>

export module Ortha.Gfx.IndexBuffer;

export namespace Ortha::Gfx {

	enum class IndexBufferType : uint8_t {
		Transient
	};

	struct IndexBuffer {
		std::vector<uint8_t> data;
		IndexBufferType type{};
		bool is32Bit{ false };
	};

} // namespace Ortha::Gfx
