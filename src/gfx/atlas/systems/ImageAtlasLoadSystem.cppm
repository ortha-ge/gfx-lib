module;

#include <entt/fwd.hpp>

export module Gfx.ImageAtlasLoadSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Gfx {

	class ImageAtlasLoadSystem {
	public:

		ImageAtlasLoadSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~ImageAtlasLoadSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx
