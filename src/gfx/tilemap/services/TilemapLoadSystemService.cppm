export module Ortha.Gfx.TilemapLoadSystemService;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.TilemapLoadSystem;

export namespace Ortha::Gfx {

	struct TilemapLoadSystemService
		: kgr::single_service<TilemapLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
