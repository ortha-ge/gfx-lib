export module Gfx.Reflection.TextureCoordinates;

export import Gfx.TextureCoordinates;

import Core.ReflectionContext;
import Glm.Reflection.Vec2;

template<>
void Core::reflect<Gfx::TextureCoordinates>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<TextureCoordinates>("TextureCoordinates")
		.property("bottomLeft", &TextureCoordinates::bottomLeft)
		.property("topRight", &TextureCoordinates::topRight)
		.build();
} // Gfx
