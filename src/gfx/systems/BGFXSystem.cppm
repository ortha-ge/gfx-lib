module;

#include <bgfx/bgfx.h>
#include <entt/fwd.hpp>

export module Gfx.BGFXSystem;

export namespace Gfx {

	class BGFXSystem {
	public:

		BGFXSystem();

		void initSystem(entt::registry& registry);
		void destroySystem(entt::registry& registry);
		void tickSystem(entt::registry& registry);

	private:

		void onWindowInternalCreated(entt::registry&, entt::entity);

		bgfx::UniformHandle mTexColourUniformHandle{};
		bgfx::UniformHandle mAlphaColourUniformHandle{};

	};

} // Gfx