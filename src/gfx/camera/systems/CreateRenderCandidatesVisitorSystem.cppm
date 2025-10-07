export module Ortha.Gfx.CreateRenderCandidatesVisitorSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx {

	class CreateRenderCandidatesVisitorSystem {
	public:

		CreateRenderCandidatesVisitorSystem(entt::registry&, Core::Scheduler&);
		~CreateRenderCandidatesVisitorSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Ortha::Gfx
