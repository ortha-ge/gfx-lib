export module Gfx.SpriteRenderSystem;

import Core.Scheduler;
import entt;

export namespace Gfx {

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

} // namespace Gfx
