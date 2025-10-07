module;

export module Ortha.Gfx.FontRenderSystemService;

export import Ortha.Gfx.FontRenderSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct FontRenderSystemService
		: kgr::single_service<FontRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
