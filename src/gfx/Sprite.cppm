module;

#include <vector>

export module Gfx.Sprite;

import Gfx.TextureCoordinates;

export namespace Gfx {

	struct Sprite {
		std::vector<TextureCoordinates> frames{};
	};

} // namespace Gfx
