export module Gfx.TilemapRenderSystem;

import Core.Scheduler;
import entt;

export namespace Gfx {

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

} // namespace Gfx
