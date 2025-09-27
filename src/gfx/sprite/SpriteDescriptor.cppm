module;

#include <vector>

export module Gfx.SpriteDescriptor;

import Gfx.TextureCoordinates;

export namespace Gfx {

	struct SpriteDescriptor {
		bool operator == (const SpriteDescriptor&) const = default;
		std::vector<TextureCoordinates> frames{};
	};

} // namespace Gfx
