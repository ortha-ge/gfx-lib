module;

#include <vector>

export module Gfx.SpriteDescriptor;

import Gfx.TextureCoordinates;

export namespace Gfx {

	struct SpriteDescriptor {
		std::vector<TextureCoordinates> frames{};
	};

} // namespace Gfx
