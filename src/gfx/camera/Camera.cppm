export module Gfx.Camera;

import entt;

export namespace Gfx {

	struct Camera {
		entt::entity viewport { entt::null };
		entt::entity sceneRoot{ entt::null };
		float fov{ 60.0f };
	};

} // namespace Gfx
