export module Gfx.CreateRenderCandidatesVisitorSystem;

import Core.Scheduler;
import entt;

export namespace Gfx {

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

} // namespace Gfx
