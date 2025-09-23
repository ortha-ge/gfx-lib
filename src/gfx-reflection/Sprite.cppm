export module Gfx.Reflection.Sprite;

export import Gfx.SpriteDescriptor;

import Core.ClassReflection;
import Core.ReflectionContext;
import Gfx.Reflection.TextureCoordinates;

template<>
void Core::reflect<Gfx::SpriteDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<SpriteDescriptor>("SpriteDescriptor")
		.property("frames", &SpriteDescriptor::frames)
		.build();
}
