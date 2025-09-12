module;

#include <glm/vec2.hpp>

export module Gfx.ViewportDescriptor;

export namespace Gfx {

	struct ViewportDescriptor {
		glm::vec2 offset{ 0.0f, 0.0f };
		glm::vec2 dimensions{ 1.0f, 1.0f };
	};

} // namespace Gfx
