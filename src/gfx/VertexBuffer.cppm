module;

#include <cstdint>
#include <vector>

export module Gfx.VertexBuffer;

export namespace Gfx {

	enum class VertexBufferType : uint8_t {
		Transient
	};

	struct VertexBuffer {
		std::vector<uint8_t> data;
		VertexBufferType type{};
	};

} // namespace Gfx
