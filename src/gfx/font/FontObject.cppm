module;

#include <string>

export module Gfx.FontObject;

import Core.ResourceHandle;

export namespace Gfx {

	struct FontObject {
		std::string text;
		Core::ResourceHandlePtr font;
	};

} // namespace Gfx
