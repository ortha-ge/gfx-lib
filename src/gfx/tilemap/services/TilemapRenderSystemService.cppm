export module Gfx.TilemapRenderSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.TilemapRenderSystem;

export namespace Gfx {

	struct TilemapRenderSystemService
		: kgr::single_service<TilemapRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
