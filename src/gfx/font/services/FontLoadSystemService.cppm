module;

export module Ortha.Gfx.FontLoadSystemService;

export import Ortha.Gfx.FontLoadSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct FontLoadSystemService
		: kgr::single_service<FontLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
