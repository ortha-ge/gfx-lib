module;

#include <string>
#include <vector>

#include <glm/vec2.hpp>

export module Gfx.ImageAtlas;

export import Gfx.ImageAtlasDescriptor;

import Core.ResourceHandle;

export namespace Gfx {

	struct ImageAtlas {
		Core::ResourceHandlePtr image;
		std::vector<ImageAtlasRegion> regions;
	};

} // namespace Gfx
