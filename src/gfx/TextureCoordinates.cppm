export module Gfx.TextureCoordinates;

import glm;

export namespace Gfx {

	struct TextureCoordinates {
		bool operator==(const TextureCoordinates&) const = default;
		glm::vec2 bottomLeft{ 0.0f, 0.0f };
		glm::vec2 topRight{ 1.0f, 1.0f };
	};

} // namespace Gfx
