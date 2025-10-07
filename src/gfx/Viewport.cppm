export module Ortha.Gfx.Viewport;

import entt;
import glm;

export namespace Ortha::Gfx {

	struct Viewport {
		entt::entity renderTarget{ entt::null };

		glm::vec2 offset{ 0.0f, 0.0f };
		glm::vec2 dimensions{ 1.0f, 1.0f };
	};

} // namespace Ortha::Gfx
