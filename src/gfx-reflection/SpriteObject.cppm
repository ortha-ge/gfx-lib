export module Gfx.Reflection.SpriteObject;

export import Gfx.SpriteObject;

import Core.EnTTComponentAttribute;
import Core.ClassReflection;
import Core.ReflectionContext;
import Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Gfx::SpriteObject>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<SpriteObject>("Sprite")
		.property("currentFrame", &SpriteObject::currentFrame)
		.annotate(createEnTTComponentAttribute<SpriteObject>())
		.build();
}
