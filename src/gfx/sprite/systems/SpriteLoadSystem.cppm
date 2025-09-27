module;

#include <entt/fwd.hpp>

export module Gfx.SpriteLoadSystem;

import Core.EnTTRegistry;
import Core.Scheduler;
import Gfx.SpriteDescriptor;

export namespace Gfx {

	class SpriteLoadSystem {
	public:

		SpriteLoadSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~SpriteLoadSystem();

		static void tickSystem(entt::registry&);
		static void tryCreateSprite(entt::registry&, entt::entity, const SpriteDescriptor&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx
