module;

#include <entt/fwd.hpp>

export module Gfx.SpriteRenderSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Gfx {

	class SpriteRenderSystem {
	public:

		SpriteRenderSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~SpriteRenderSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle{};

	};

} // namespace Gfx
