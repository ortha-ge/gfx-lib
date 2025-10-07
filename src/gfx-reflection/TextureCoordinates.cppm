export module Gfx.Reflection.TextureCoordinates;

export import Gfx.TextureCoordinates;

import Core.Reflect;
import Glm.Reflection.Vec2;

template<>
void Core::reflect<Gfx::TextureCoordinates>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<TextureCoordinates>("TextureCoordinates")
		.field<&TextureCoordinates::bottomLeft>("bottomLeft")
		.field<&TextureCoordinates::topRight>("topRight")
		;
} // Gfx
