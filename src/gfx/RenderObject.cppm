module;

#include <memory>
#include <variant>

export module Ortha.Gfx.RenderObject;

import Ortha.Core.ResourceHandle;
import Ortha.Gfx.MaterialDescriptor;

export namespace Ortha::Gfx {

	struct RenderObject {
		std::variant<MaterialDescriptor, std::shared_ptr<Core::ResourceHandle>> materialResource{};
	};

} // namespace Ortha::Gfx
