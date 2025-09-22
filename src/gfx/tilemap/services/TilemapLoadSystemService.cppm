export module Gfx.TilemapLoadSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.TilemapLoadSystem;

export namespace Gfx {

	struct TilemapLoadSystemService
		: kgr::single_service<TilemapLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
