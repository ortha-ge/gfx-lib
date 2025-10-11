export module Ortha.Gfx.Reflection.ImageAtlas;

export import Ortha.Gfx.ImageAtlasDescriptor;

import Ortha.Gfx.Reflection.TextureCoordinates;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::ImageAtlasRegion>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflectionContext.addClass<ImageAtlasRegion>("ImageAtlasRegion")
			.field<&ImageAtlasRegion::name>("name")
			.field<&ImageAtlasRegion::textureCoordinates>("textureCoordinates");
	}

	template<>
	void reflect<Gfx::ImageAtlasDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;

		reflect<ImageAtlasRegion>(reflectionContext);

		reflectionContext.addClass<ImageAtlasDescriptor>("ImageAtlas")
			.field<&ImageAtlasDescriptor::imageFilePath>("imageFilePath")
			.field<&ImageAtlasDescriptor::regions>("regions");
	}

}
