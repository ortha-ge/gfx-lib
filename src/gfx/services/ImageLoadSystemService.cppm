
export module Ortha.Gfx.ImageLoadSystemService;

export import Ortha.Gfx.ImageLoadSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct ImageLoadSystemService
		: kgr::single_service<ImageLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
