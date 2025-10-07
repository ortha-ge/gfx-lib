export module Gfx.Reflection.Sprite;

export import Gfx.SpriteDescriptor;

import Core.Reflect;
import Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Gfx::SpriteDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<SpriteDescriptor>("SpriteDescriptor")
		.field<&SpriteDescriptor::frames>("frames")
	;
}
