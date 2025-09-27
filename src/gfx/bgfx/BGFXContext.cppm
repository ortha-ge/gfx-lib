module;

#include <bgfx/bgfx.h>
#include <entt/entity/entity.hpp>

export module Gfx.BGFX.BGFXContext;

export namespace Gfx::BGFX {

	struct BGFXContext {
		entt::entity defaultWindow{ entt::null };
	};

} // namespace Gfx::BGFX
