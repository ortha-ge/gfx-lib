export module Ortha.Gfx.Reflection.SpriteObject;

export import Ortha.Gfx.SpriteObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;
import Ortha.Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Ortha::Gfx::SpriteObject>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<SpriteObject>("Sprite")
		.field<&SpriteObject::currentFrame>("currentFrame")
		//.annotate(createEnTTComponentAttribute<SpriteObject>())
		;
}
