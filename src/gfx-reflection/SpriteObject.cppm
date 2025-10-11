export module Ortha.Gfx.Reflection.SpriteObject;

export import Ortha.Gfx.SpriteObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Gfx.Reflection.TextureCoordinates;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::SpriteObject>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<SpriteObject>("Sprite")
			.field<&SpriteObject::currentFrame>("currentFrame");
			//.annotate(createEnTTComponentAttribute<SpriteObject>())
	}

}
