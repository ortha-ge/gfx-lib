export module Ortha.Gfx.BGFX.Systems;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx::BGFX {

	class BGFXSystems {
	public:

		BGFXSystems(entt::registry&, Core::Scheduler&);
		~BGFXSystems();

		void tickSystem(entt::registry&);
		void tryCreateBGFXContext(entt::registry&);
		void onNativeWindowHandlesCreated(entt::registry&, entt::entity);

	private:

		void connectEnTTSignals(entt::registry&);

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

		entt::entity mBGFXContext{ entt::null };

	};

} // namespace Ortha::Gfx::BGFX
