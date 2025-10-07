
export module Ortha.Gfx.Reflection;

export import Ortha.Gfx.Reflection.Camera;
export import Ortha.Gfx.Reflection.FontObject;
export import Ortha.Gfx.Reflection.ImageAtlas;
export import Ortha.Gfx.Reflection.ImageDescriptor;
export import Ortha.Gfx.Reflection.MaterialDescriptor;
export import Ortha.Gfx.Reflection.Projection;
export import Ortha.Gfx.Reflection.RenderObject;
export import Ortha.Gfx.Reflection.ShaderProgramDescriptor;
export import Ortha.Gfx.Reflection.Sprite;
export import Ortha.Gfx.Reflection.SpriteObject;
export import Ortha.Gfx.Reflection.TextureCoordinates;
export import Ortha.Gfx.Reflection.Tilemap;
export import Ortha.Gfx.Reflection.TilemapObject;
export import Ortha.Gfx.Reflection.Viewport;

import Ortha.RTTI.ReflectionContext;
import Ortha.Gfx.Systems;

export namespace Ortha::Gfx {

	void reflectAll(Ortha::RTTI::ReflectionContext&);

} // namespace Ortha::Gfx
