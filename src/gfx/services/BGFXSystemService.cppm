
export module Gfx.BGFXSystemService;

export import Gfx.BGFXSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct BGFXSystemService
		: kgr::single_service<BGFXSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
