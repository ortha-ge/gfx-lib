export module Ortha.Gfx.Reflection.Sprite;

export import Ortha.Gfx.SpriteDescriptor;

import Ortha.Core.Reflect;
import Ortha.Gfx.Reflection.TextureCoordinates;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::SpriteDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<SpriteDescriptor>("SpriteDescriptor")
			.field<&SpriteDescriptor::frames>("frames")
		;
	}

}
