export module Ortha.Gfx.TilemapLoadSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx {

	class TilemapLoadSystem {
	public:

		TilemapLoadSystem(entt::registry&, Core::Scheduler&);
		~TilemapLoadSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Ortha::Gfx
