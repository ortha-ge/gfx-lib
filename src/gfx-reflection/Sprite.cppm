export module Ortha.Gfx.Reflection.Sprite;

export import Ortha.Gfx.SpriteDescriptor;

import Ortha.Gfx.Reflection.TextureCoordinates;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::SpriteDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<SpriteDescriptor>("SpriteDescriptor")
			.field<&SpriteDescriptor::frames>("frames")
		;
	}

}
