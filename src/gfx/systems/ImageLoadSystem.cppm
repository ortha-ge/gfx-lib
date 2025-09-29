module;

#include <memory>

#include <bx/allocator.h>

export module Gfx.ImageLoadSystem;

import Core.Scheduler;
import entt;

export namespace Gfx {

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

} // namespace Gfx
