module;

#include <chrono>
#include <cstdint>
#include <memory>
#include <variant>

export module Ortha.Gfx.SpriteObject;

import Ortha.Core.ResourceHandle;
import Ortha.Gfx.Sprite;

export namespace Ortha::Gfx {

	struct SpriteObject {
		std::variant<Sprite, std::shared_ptr<Core::ResourceHandle>> spriteResource{};
		uint16_t currentFrame{ 0 };
		std::chrono::time_point<std::chrono::steady_clock> lastAnimUpdateTime{};
	};

} // namespace Ortha::Gfx
