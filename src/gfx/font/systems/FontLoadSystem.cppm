export module Gfx.FontLoadSystem;

import Core.Scheduler;
import entt;

export namespace Gfx {
	class FontLoadSystem {
	public:

		FontLoadSystem(entt::registry&, Core::Scheduler&);
		~FontLoadSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};
} // namespace Gfx
