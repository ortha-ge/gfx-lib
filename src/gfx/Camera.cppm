module;

#include <entt/entt.hpp>

export module Gfx.Camera;

export namespace Gfx {

	struct Camera {
		entt::entity sceneRoot{ entt::null };
		float fov{ 60.0f };
	};

} // namespace Gfx
