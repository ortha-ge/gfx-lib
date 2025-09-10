module;

#include <chrono>
#include <cstdint>
#include <memory>

export module Gfx.SpriteObject;

import Core.ResourceHandle;

export namespace Gfx {

	struct SpriteObject {
		std::shared_ptr<Core::ResourceHandle> spriteResource{};
		uint16_t currentFrame{ 0 };
		std::chrono::time_point<std::chrono::steady_clock> lastAnimUpdateTime{};
	};

} // namespace Gfx
