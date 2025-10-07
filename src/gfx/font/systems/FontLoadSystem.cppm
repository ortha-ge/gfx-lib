export module Ortha.Gfx.FontLoadSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx {
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
} // namespace Ortha::Gfx
