export module Gfx.BGFX.BGFXResourceCreationSystemService;

export import Gfx.BGFX.BGFXResourceCreationSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx::BGFX {

	struct BGFXResourceCreationSystemService
		: kgr::single_service<
			  BGFXResourceCreationSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx::BGFX
