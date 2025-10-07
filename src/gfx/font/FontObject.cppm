module;

#include <string>

export module Ortha.Gfx.FontObject;

import Ortha.Core.ResourceHandle;

export namespace Ortha::Gfx {

	struct FontObject {
		std::string text;
		Core::ResourceHandlePtr font;
	};

} // namespace Ortha::Gfx
