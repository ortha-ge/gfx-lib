module;

#include <string>
#include <vector>

#include <glm/vec2.hpp>

export module Ortha.Gfx.ImageAtlasDescriptor;

import Ortha.Gfx.TextureCoordinates;

export namespace Ortha::Gfx {

	struct ImageAtlasRegion {
		std::string name;
		TextureCoordinates textureCoordinates;
	};

	struct ImageAtlasDescriptor {
		std::string imageFilePath;
		std::vector<ImageAtlasRegion> regions;
	};

} // namespace Ortha::Gfx
