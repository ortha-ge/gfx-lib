export module Ortha.Gfx.Reflection.ImageAtlas;

export import Ortha.Gfx.ImageAtlasDescriptor;

import Ortha.Core.Reflect;
import Ortha.Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Ortha::Gfx::ImageAtlasRegion>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;

	reflectionContext.addClass<ImageAtlasRegion>("ImageAtlasRegion")
		.field<&ImageAtlasRegion::name>("name")
		.field<&ImageAtlasRegion::textureCoordinates>("textureCoordinates")
		;
}

template<>
void Core::reflect<Ortha::Gfx::ImageAtlasDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;

	reflect<ImageAtlasRegion>(reflectionContext);

	reflectionContext.addClass<ImageAtlasDescriptor>("ImageAtlas")
		.field<&ImageAtlasDescriptor::imageFilePath>("imageFilePath")
		.field<&ImageAtlasDescriptor::regions>("regions")
		;
}
