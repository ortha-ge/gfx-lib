export module Ortha.Gfx.Reflection.TextureCoordinates;

export import Ortha.Gfx.TextureCoordinates;

import Ortha.Core.Reflect;
import Glm.Reflection.Vec2;

template<>
void Core::reflect<Ortha::Gfx::TextureCoordinates>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<TextureCoordinates>("TextureCoordinates")
		.field<&TextureCoordinates::bottomLeft>("bottomLeft")
		.field<&TextureCoordinates::topRight>("topRight")
		;
} // Gfx
