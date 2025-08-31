module;

#include <list>
#include <utility>

#include <bgfx/bgfx.h>
#include <entt/entity/entity.hpp>

export module Gfx.BGFXContext;

export namespace Gfx {

	struct BGFXContext {
		std::list<std::pair<entt::entity, bgfx::TextureHandle>> trackedTextureHandles{};
		std::list<std::pair<entt::entity, bgfx::ProgramHandle>> trackedShaderProgramHandles{};
		std::list<std::pair<entt::entity, bgfx::ShaderHandle>> trackedShaderHandles{};
	};

} // Gfx