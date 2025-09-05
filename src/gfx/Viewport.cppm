module;

#include <entt/entity/entity.hpp>

export module Gfx.Viewport;



export namespace Gfx {

	struct Viewport {
		entt::entity camera{ entt::null };
	};

} // namespace Gfx
