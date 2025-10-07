export module Ortha.Gfx.Reflection.ImageAtlas;

export import Ortha.Gfx.ImageAtlasDescriptor;

import Ortha.Core.Reflect;
import Ortha.Gfx.Reflection.TextureCoordinates;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::ImageAtlasRegion>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<ImageAtlasRegion>("ImageAtlasRegion")
			.field<&ImageAtlasRegion::name>("name")
			.field<&ImageAtlasRegion::textureCoordinates>("textureCoordinates")
			;
	}

	template<>
	void reflect<Gfx::ImageAtlasDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflect<ImageAtlasRegion>(reflectionContext);

		reflectionContext.addClass<ImageAtlasDescriptor>("ImageAtlas")
			.field<&ImageAtlasDescriptor::imageFilePath>("imageFilePath")
			.field<&ImageAtlasDescriptor::regions>("regions")
			;
	}

}
