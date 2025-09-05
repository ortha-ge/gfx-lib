module;

#include <entt/entity/entity.hpp>

export module Gfx.Viewport;

export namespace Gfx {

	struct Viewport {
		entt::entity camera{ entt::null };
		float x{ 0.0f };
		float y{ 0.0f };
		float width{ 1.0f };
		float height{ 1.0f };
	};

} // namespace Gfx
