module;

#include <functional>

#include <entt/fwd.hpp>

export module Gfx.BGFXDrawCallback;

export namespace Gfx {

	struct BGFXDrawCallback {
		std::function<void(entt::registry&)> drawCallback;
	};

} // namespace Gfx
