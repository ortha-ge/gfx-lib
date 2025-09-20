module;

export module Gfx.FontRenderSystemService;

export import Gfx.FontRenderSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct FontRenderSystemService
		: kgr::single_service<FontRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
