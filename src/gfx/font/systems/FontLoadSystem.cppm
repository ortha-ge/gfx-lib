module;

#include <entt/fwd.hpp>

export module Gfx.FontLoadSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Gfx {
	class FontLoadSystem {
	public:

		FontLoadSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~FontLoadSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};
} // namespace Gfx
