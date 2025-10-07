export module Ortha.Gfx.SpriteRenderSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx {

	class SpriteRenderSystem {
	public:

		SpriteRenderSystem(entt::registry&, Core::Scheduler&);
		~SpriteRenderSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle{};

	};

} // namespace Ortha::Gfx
