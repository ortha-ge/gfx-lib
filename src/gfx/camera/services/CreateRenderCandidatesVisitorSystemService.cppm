export module Ortha.Gfx.CreateRenderCandidatesVisitorSystemService;

export import Ortha.Gfx.CreateRenderCandidatesVisitorSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct CreateRenderCandidatesVisitorSystemService
		: kgr::single_service<
			  CreateRenderCandidatesVisitorSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {
	};

}