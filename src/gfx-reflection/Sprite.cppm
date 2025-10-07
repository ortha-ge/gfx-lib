export module Ortha.Gfx.Reflection.Sprite;

export import Ortha.Gfx.SpriteDescriptor;

import Ortha.Core.Reflect;
import Ortha.Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Ortha::Gfx::SpriteDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<SpriteDescriptor>("SpriteDescriptor")
		.field<&SpriteDescriptor::frames>("frames")
	;
}
