export module Gfx.TilemapLoadSystem;

import Core.Scheduler;
import entt;

export namespace Gfx {

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

} // namespace Gfx
