export module Ortha.Gfx.SpriteLoadSystem;

import Ortha.Core.Scheduler;
import Ortha.Gfx.SpriteDescriptor;
import entt;

export namespace Ortha::Gfx {

	class SpriteLoadSystem {
	public:

		SpriteLoadSystem(entt::registry&, Core::Scheduler&);
		~SpriteLoadSystem();

		static void tickSystem(entt::registry&);
		static void tryCreateSprite(entt::registry&, entt::entity, const SpriteDescriptor&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Ortha::Gfx
