export module Ortha.Gfx.ImageAtlasLoadSystemService;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.ImageAtlasLoadSystem;

export namespace Ortha::Gfx {

	struct ImageAtlasLoadSystemService
		: kgr::single_service<ImageAtlasLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
