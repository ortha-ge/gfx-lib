export module Gfx.ImageAtlasLoadSystem;

import Core.Scheduler;
import entt;

export namespace Gfx {

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

} // namespace Gfx
