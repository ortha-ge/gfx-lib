export module Ortha.Gfx.ViewportDescriptor;

import glm;

export namespace Ortha::Gfx {

	struct ViewportDescriptor {
		glm::vec2 offset{ 0.0f, 0.0f };
		glm::vec2 dimensions{ 1.0f, 1.0f };
	};

} // namespace Ortha::Gfx
