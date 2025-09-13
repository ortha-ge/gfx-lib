module;

#include <memory>
#include <variant>

export module Gfx.RenderObject;

import Core.ResourceHandle;
import Gfx.MaterialDescriptor;

export namespace Gfx {

	struct RenderObject {
		std::variant<MaterialDescriptor, std::shared_ptr<Core::ResourceHandle>> materialResource{};
	};

} // namespace Gfx
