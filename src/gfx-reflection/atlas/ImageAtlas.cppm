export module Gfx.Reflection.ImageAtlas;

export import Gfx.ImageAtlasDescriptor;

import Core.Reflect;
import Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Gfx::ImageAtlasRegion>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflectionContext.addClass<ImageAtlasRegion>("ImageAtlasRegion")
		.field<&ImageAtlasRegion::name>("name")
		.field<&ImageAtlasRegion::textureCoordinates>("textureCoordinates")
		;
}

template<>
void Core::reflect<Gfx::ImageAtlasDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;

	reflect<ImageAtlasRegion>(reflectionContext);

	reflectionContext.addClass<ImageAtlasDescriptor>("ImageAtlas")
		.field<&ImageAtlasDescriptor::imageFilePath>("imageFilePath")
		.field<&ImageAtlasDescriptor::regions>("regions")
		;
}
