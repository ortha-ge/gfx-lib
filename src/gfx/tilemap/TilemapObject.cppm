export module Ortha.Gfx.TilemapObject;

import Ortha.Core.ResourceHandle;

export namespace Ortha::Gfx {

	struct TilemapObject {
		Core::ResourceHandlePtr tilemap;
		Core::ResourceHandlePtr shaderProgram;
	};

} // namespace Ortha::Gfx
