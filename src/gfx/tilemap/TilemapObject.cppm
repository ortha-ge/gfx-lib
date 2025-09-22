export module Gfx.TilemapObject;

import Core.ResourceHandle;

export namespace Gfx {

	struct TilemapObject {
		Core::ResourceHandlePtr tilemap;
		Core::ResourceHandlePtr shaderProgram;
	};

} // namespace Gfx
