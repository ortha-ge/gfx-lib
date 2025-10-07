module;

#include <cstdint>
#include <vector>

export module Ortha.Gfx.VertexBuffer;

import entt;

export namespace Ortha::Gfx {

	enum class VertexBufferType : uint8_t {
		Transient
	};

	struct VertexBuffer {
		std::vector<uint8_t> data;
		VertexBufferType type{};
		entt::entity vertexLayout{ entt::null };
	};

} // namespace Ortha::Gfx
