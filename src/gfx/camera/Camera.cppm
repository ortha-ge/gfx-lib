export module Ortha.Gfx.Camera;

import entt;

export namespace Ortha::Gfx {

	struct Camera {
		entt::entity viewport { entt::null };
		entt::entity sceneRoot{ entt::null };
		float fov{ 60.0f };
	};

} // namespace Ortha::Gfx
