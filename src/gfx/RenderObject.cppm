module;

#include <chrono>
#include <entt/entity/entity.hpp>

export module Gfx.RenderObject;

export namespace Gfx {

	struct RenderObject {
		entt::entity materialResource{ entt::null };
		uint16_t currentSpriteFrame{ 0 };
		std::chrono::time_point<std::chrono::steady_clock> lastAnimUpdateTime{};
	};

} // namespace Gfx
