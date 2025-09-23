module Gfx.Reflection;

namespace Gfx {

	void reflectAll(Core::ReflectionContext& reflectionContext) {
		using namespace Core;

		reflect<CameraDescriptor>(reflectionContext);
		reflect<Camera>(reflectionContext);
		reflect<FontObject>(reflectionContext);
		reflect<ImageAtlasDescriptor>(reflectionContext);
		reflect<ImageDescriptor>(reflectionContext);
		reflect<MaterialDescriptor>(reflectionContext);
		reflect<Projection>(reflectionContext);
		reflect<RenderObject>(reflectionContext);
		reflect<ShaderProgramDescriptor>(reflectionContext);
		reflect<SpriteDescriptor>(reflectionContext);
		reflect<SpriteObject>(reflectionContext);
		reflect<TextureCoordinates>(reflectionContext);
		reflect<TilemapDescriptor>(reflectionContext);
		reflect<TilemapObject>(reflectionContext);
		reflect<ViewportDescriptor>(reflectionContext);
		reflect<Viewport>(reflectionContext);
	}

} // namespace Gfx
