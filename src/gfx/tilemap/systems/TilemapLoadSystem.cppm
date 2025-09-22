module;

#include <entt/fwd.hpp>

export module Gfx.TilemapLoadSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Gfx {

	class TilemapLoadSystem {
	public:

		TilemapLoadSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~TilemapLoadSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx
