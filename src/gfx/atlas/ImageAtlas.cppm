module;

#include <string>
#include <vector>

#include <glm/vec2.hpp>

export module Ortha.Gfx.ImageAtlas;

export import Ortha.Gfx.ImageAtlasDescriptor;

import Ortha.Core.ResourceHandle;

export namespace Ortha::Gfx {

	struct ImageAtlas {
		Core::ResourceHandlePtr image;
		std::vector<ImageAtlasRegion> regions;
	};

} // namespace Ortha::Gfx
