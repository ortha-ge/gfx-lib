module;

#include <chrono>
#include <cstdint>

#include <entt/entity/entity.hpp>

export module Gfx.SpriteObject;

export namespace Gfx {

	struct SpriteObject {
		entt::entity spriteResource{ entt::null };
		uint16_t currentFrame{ 0 };
		std::chrono::time_point<std::chrono::steady_clock> lastAnimUpdateTime{};
	};

} // namespace Gfx
