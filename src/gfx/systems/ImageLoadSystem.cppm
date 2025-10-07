module;

#include <memory>

#include <bx/allocator.h>

export module Ortha.Gfx.ImageLoadSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx {

	class ImageLoadSystem {
	public:
		ImageLoadSystem(entt::registry&, Core::Scheduler&);
		~ImageLoadSystem();

		static void tickSystem(entt::registry& registry);

	private:
		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle{};

		std::unique_ptr<bx::AllocatorI> mAllocator{};
	};

} // namespace Ortha::Gfx
