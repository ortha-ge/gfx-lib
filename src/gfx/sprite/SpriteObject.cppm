module;

#include <chrono>
#include <cstdint>
#include <memory>
#include <variant>

export module Gfx.SpriteObject;

import Core.ResourceHandle;
import Gfx.Sprite;

export namespace Gfx {

	struct SpriteObject {
		std::variant<Sprite, std::shared_ptr<Core::ResourceHandle>> spriteResource{};
		uint16_t currentFrame{ 0 };
		std::chrono::time_point<std::chrono::steady_clock> lastAnimUpdateTime{};
	};

} // namespace Gfx
