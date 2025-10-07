export module Ortha.Gfx.Reflection.TextureCoordinates;

export import Ortha.Gfx.TextureCoordinates;

import Ortha.Core.Reflect;
import Glm.Reflection.Vec2;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::TextureCoordinates>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<TextureCoordinates>("TextureCoordinates")
			.field<&TextureCoordinates::bottomLeft>("bottomLeft")
			.field<&TextureCoordinates::topRight>("topRight")
			;
	} // Gfx

}
