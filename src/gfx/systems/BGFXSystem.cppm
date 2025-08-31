module;

#include <bgfx/bgfx.h>
#include <entt/fwd.hpp>

export module Gfx.BGFXSystem;

import Core.EnTTRegistry;
import Gfx.BGFXContext;

export namespace Gfx {

	class BGFXSystem {
	public:

		BGFXSystem(Core::EnTTRegistry&);
		~BGFXSystem();

		void initSystem(entt::registry& registry);
		void destroySystem(entt::registry& registry);
		void tickSystem(entt::registry& registry);

	private:

		void onWindowInternalCreated(entt::registry&, entt::entity);
		static void _destroyDroppedTextures(const entt::registry&, BGFXContext& context);
		void _destroyTrackedTextures(BGFXContext& context) const;

		bgfx::UniformHandle mTexColourUniformHandle{};
		bgfx::UniformHandle mAlphaColourUniformHandle{};

		Core::EnTTRegistry& mRegistry;

	};

} // Gfx