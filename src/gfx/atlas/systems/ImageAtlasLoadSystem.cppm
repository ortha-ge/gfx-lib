export module Ortha.Gfx.ImageAtlasLoadSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx {

	class ImageAtlasLoadSystem {
	public:

		ImageAtlasLoadSystem(entt::registry&, Core::Scheduler&);
		~ImageAtlasLoadSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Ortha::Gfx
