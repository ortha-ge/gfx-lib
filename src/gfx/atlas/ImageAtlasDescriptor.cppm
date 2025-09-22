module;

#include <string>
#include <vector>

#include <glm/vec2.hpp>

export module Gfx.ImageAtlasDescriptor;

import Gfx.TextureCoordinates;

export namespace Gfx {

	struct ImageAtlasRegion {
		std::string name;
		TextureCoordinates textureCoordinates;
	};

	struct ImageAtlasDescriptor {
		std::string imageFilePath;
		std::vector<ImageAtlasRegion> regions;
	};

} // namespace Gfx
