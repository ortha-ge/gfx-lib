module;

#include <entt/entity/entity.hpp>
#include <glm/vec2.hpp>

export module Gfx.Viewport;

export namespace Gfx {

	struct Viewport {
		entt::entity camera{ entt::null };

		glm::vec2 offset{ 0.0f, 0.0f };
		glm::vec2 dimensions{ 1.0f, 1.0f };
	};

} // namespace Gfx
