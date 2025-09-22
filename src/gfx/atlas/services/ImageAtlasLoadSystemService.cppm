export module Gfx.ImageAtlasLoadSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.ImageAtlasLoadSystem;

export namespace Gfx {

	struct ImageAtlasLoadSystemService
		: kgr::single_service<ImageAtlasLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
