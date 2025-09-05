module;

#include <bx/allocator.h>
#include <entt/fwd.hpp>

export module Gfx.ImageLoadSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Gfx {

	class ImageLoadSystem {
	public:

		ImageLoadSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~ImageLoadSystem();

		static void tickSystem(entt::registry& registry);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle{};

		std::unique_ptr<bx::AllocatorI> mAllocator{};
	};

} // Core
