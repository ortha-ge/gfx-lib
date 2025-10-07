export module Ortha.Gfx.TilemapRenderSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx {

	class TilemapRenderSystem {
	public:

		TilemapRenderSystem(entt::registry&, Core::Scheduler&);
		~TilemapRenderSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Ortha::Gfx
