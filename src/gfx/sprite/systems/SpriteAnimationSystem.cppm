module;

#include <chrono>

export module Ortha.Gfx.SpriteAnimationSystem;

import Ortha.Core.Scheduler;
import Ortha.Gfx.Sprite;
import Ortha.Gfx.SpriteObject;
import entt;

export namespace Ortha::Gfx {

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

} // namespace Ortha::Gfx
