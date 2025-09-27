module;

#include <chrono>

#include <entt/fwd.hpp>

export module Gfx.SpriteAnimationSystem;

import Core.EnTTRegistry;
import Core.Scheduler;
import Gfx.Sprite;
import Gfx.SpriteObject;

export namespace Gfx {

	class SpriteAnimationSystem {
	public:

		SpriteAnimationSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~SpriteAnimationSystem();

		static void tickSystem(entt::registry&);
		static void tryTickSpriteAnimation(entt::registry&, entt::entity, SpriteObject&, std::chrono::steady_clock::time_point);
		static void tickSpriteAnimation(const Sprite&, SpriteObject&, std::chrono::steady_clock::time_point);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx
