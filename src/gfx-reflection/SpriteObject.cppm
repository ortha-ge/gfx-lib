export module Gfx.Reflection.SpriteObject;

export import Gfx.SpriteObject;

import Core.EnTTComponentAttribute;
import Core.Reflect;
import Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Gfx::SpriteObject>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<SpriteObject>("Sprite")
		.field<&SpriteObject::currentFrame>("currentFrame")
		//.annotate(createEnTTComponentAttribute<SpriteObject>())
		;
}
