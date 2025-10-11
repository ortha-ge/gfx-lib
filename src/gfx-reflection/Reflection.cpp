module Ortha.Gfx.Reflection;

import Ortha.RTTI.Reflect;

namespace Ortha::Gfx {

	void reflectAll(RTTI::ReflectionContext& reflectionContext) {
		using namespace Ortha::RTTI;

		ensureReflected<CameraDescriptor>(reflectionContext);
		ensureReflected<Camera>(reflectionContext);
		ensureReflected<FontObject>(reflectionContext);
		ensureReflected<ImageAtlasDescriptor>(reflectionContext);
		ensureReflected<ImageDescriptor>(reflectionContext);
		ensureReflected<MaterialDescriptor>(reflectionContext);
		//ensureReflected<Projection>(reflectionContext);
		ensureReflected<RenderObject>(reflectionContext);
		ensureReflected<ShaderProgramDescriptor>(reflectionContext);
		ensureReflected<SpriteDescriptor>(reflectionContext);
		ensureReflected<SpriteObject>(reflectionContext);
		//ensureReflected<TextureCoordinates>(reflectionContext);
		ensureReflected<TilemapDescriptor>(reflectionContext);
		ensureReflected<TilemapObject>(reflectionContext);
		ensureReflected<ViewportDescriptor>(reflectionContext);
		ensureReflected<Viewport>(reflectionContext);
	}

} // namespace Ortha::Gfx
