module;

#include <memory>

export module Gfx.RenderObject;

import Core.ResourceHandle;

export namespace Gfx {

	struct RenderObject {
		std::shared_ptr<Core::ResourceHandle> materialResource{};
	};

} // namespace Gfx
