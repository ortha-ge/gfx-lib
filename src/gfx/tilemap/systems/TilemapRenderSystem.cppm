module;

#include <entt/fwd.hpp>

export module Gfx.TilemapRenderSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Gfx {

	class TilemapRenderSystem {
	public:

		TilemapRenderSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~TilemapRenderSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx
