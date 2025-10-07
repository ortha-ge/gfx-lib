export module Ortha.Gfx.BGFX.BGFXResourceCreationSystemService;

export import Ortha.Gfx.BGFX.BGFXResourceCreationSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx::BGFX {

	struct BGFXResourceCreationSystemService
		: kgr::single_service<
			  BGFXResourceCreationSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx::BGFX
