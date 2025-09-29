module;

#include <chrono>

export module Gfx.SpriteAnimationSystem;

import Core.Scheduler;
import Gfx.Sprite;
import Gfx.SpriteObject;
import entt;

export namespace Gfx {

	class SpriteAnimationSystem {
	public:

		SpriteAnimationSystem(entt::registry&, Core::Scheduler&);
		~SpriteAnimationSystem();

		static void tickSystem(entt::registry&);
		static void tryTickSpriteAnimation(entt::registry&, entt::entity, SpriteObject&, std::chrono::steady_clock::time_point);
		static void tickSpriteAnimation(const Sprite&, SpriteObject&, std::chrono::steady_clock::time_point);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx
