module;

export module Gfx.FontLoadSystemService;

export import Gfx.FontLoadSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct FontLoadSystemService
		: kgr::single_service<FontLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
