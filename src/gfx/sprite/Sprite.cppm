module;

#include <vector>

export module Ortha.Gfx.Sprite;

import Ortha.Gfx.SpriteDescriptor;
import Ortha.Gfx.TextureCoordinates;

export namespace Ortha::Gfx {

	struct Sprite {
		bool operator == (const Sprite&) const = default;
		SpriteDescriptor descriptor;
	};

} // namespace Ortha::Gfx
