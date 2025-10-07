export module Ortha.Gfx.Reflection.SpriteObject;

export import Ortha.Gfx.SpriteObject;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;
import Ortha.Gfx.Reflection.TextureCoordinates;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::SpriteObject>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<SpriteObject>("Sprite")
			.field<&SpriteObject::currentFrame>("currentFrame")
			//.annotate(createEnTTComponentAttribute<SpriteObject>())
			;
	}

}
