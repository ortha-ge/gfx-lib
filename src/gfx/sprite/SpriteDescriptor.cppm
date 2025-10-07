module;

#include <vector>

export module Ortha.Gfx.SpriteDescriptor;

import Ortha.Gfx.TextureCoordinates;

export namespace Ortha::Gfx {

	struct SpriteDescriptor {
		bool operator == (const SpriteDescriptor&) const = default;
		std::vector<TextureCoordinates> frames{};
	};

} // namespace Ortha::Gfx
