module;

#include <entt/entt.hpp>

export module Gfx.BGFX.Systems;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Gfx::BGFX {

	class BGFXSystems {
	public:

		BGFXSystems(Core::EnTTRegistry&, Core::Scheduler&);
		~BGFXSystems();

		void tickSystem(entt::registry&);
		void tryCreateBGFXContext(entt::registry&);
		void onNativeWindowHandlesCreated(entt::registry&, entt::entity);

	private:

		void connectEnTTSignals(entt::registry&);

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

		entt::entity mBGFXContext{ entt::null };

	};

} // namespace Gfx::BGFX
