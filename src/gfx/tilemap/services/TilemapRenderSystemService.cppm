export module Ortha.Gfx.TilemapRenderSystemService;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.TilemapRenderSystem;

export namespace Ortha::Gfx {

	struct TilemapRenderSystemService
		: kgr::single_service<TilemapRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
