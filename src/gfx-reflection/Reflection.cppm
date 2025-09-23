
export module Gfx.Reflection;

export import Gfx.Reflection.Camera;
export import Gfx.Reflection.FontObject;
export import Gfx.Reflection.ImageAtlas;
export import Gfx.Reflection.ImageDescriptor;
export import Gfx.Reflection.MaterialDescriptor;
export import Gfx.Reflection.Projection;
export import Gfx.Reflection.RenderObject;
export import Gfx.Reflection.ShaderProgramDescriptor;
export import Gfx.Reflection.Sprite;
export import Gfx.Reflection.SpriteObject;
export import Gfx.Reflection.TextureCoordinates;
export import Gfx.Reflection.Tilemap;
export import Gfx.Reflection.TilemapObject;
export import Gfx.Reflection.Viewport;

import Core.ReflectionContext;
import Gfx.Systems;

export namespace Gfx {

	void reflectAll(Core::ReflectionContext&);

} // namespace Gfx
