module;

#include <entt/entity/entity.hpp>

export module Gfx.RenderObject;

export namespace Gfx {

	struct RenderObject {
		entt::entity materialResource{ entt::null };
	};

} // namespace Gfx
