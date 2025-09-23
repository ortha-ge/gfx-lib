export module Gfx.Reflection.SpriteObject;

export import Gfx.SpriteObject;

import Core.EnTTClassAnnotation;
import Core.ClassReflection;
import Core.ReflectionContext;
import Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Gfx::SpriteObject>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<SpriteObject>("Sprite")
		.property("currentFrame", &SpriteObject::currentFrame)
		.annotate(createEnTTClassAnnotation<SpriteObject>())
		.build();
}
