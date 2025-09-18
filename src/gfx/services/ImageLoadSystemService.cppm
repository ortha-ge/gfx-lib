
export module Gfx.ImageLoadSystemService;

export import Gfx.ImageLoadSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct ImageLoadSystemService
		: kgr::single_service<ImageLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
