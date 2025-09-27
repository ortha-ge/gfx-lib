module;

#include <entt/fwd.hpp>

export module Gfx.CreateRenderCandidatesVisitorSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Gfx {

	class CreateRenderCandidatesVisitorSystem {
	public:

		CreateRenderCandidatesVisitorSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~CreateRenderCandidatesVisitorSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx
