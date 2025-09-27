export module Gfx.CreateRenderCandidatesVisitorSystemService;

export import Gfx.CreateRenderCandidatesVisitorSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct CreateRenderCandidatesVisitorSystemService
		: kgr::single_service<
			  CreateRenderCandidatesVisitorSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {
	};

}