module;

#include <vector>

export module Gfx.Sprite;

import Gfx.SpriteDescriptor;
import Gfx.TextureCoordinates;

export namespace Gfx {

	struct Sprite {
		bool operator == (const Sprite&) const = default;
		SpriteDescriptor descriptor;
	};

} // namespace Gfx
