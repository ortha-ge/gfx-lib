export module Gfx.Reflection.ImageAtlas;

export import Gfx.ImageAtlasDescriptor;

import Core.ReflectionContext;
import Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Gfx::ImageAtlasRegion>(ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<ImageAtlasRegion>("ImageAtlasRegion")
		.property("name", &ImageAtlasRegion::name)
		.property("textureCoordinates", &ImageAtlasRegion::textureCoordinates)
		.build();
}

template<>
void Core::reflect<Gfx::ImageAtlasDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<ImageAtlasDescriptor>("ImageAtlas")
		.property("imageFilePath", &ImageAtlasDescriptor::imageFilePath)
		.property("regions", &ImageAtlasDescriptor::regions)
		.build();
}
