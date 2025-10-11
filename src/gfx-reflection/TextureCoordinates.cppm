export module Ortha.Gfx.Reflection.TextureCoordinates;

export import Ortha.Gfx.TextureCoordinates;

import Glm.Reflection.Vec2;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::TextureCoordinates>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<TextureCoordinates>("TextureCoordinates")
			.field<&TextureCoordinates::bottomLeft>("bottomLeft")
			.field<&TextureCoordinates::topRight>("topRight");
	} // Gfx

}
