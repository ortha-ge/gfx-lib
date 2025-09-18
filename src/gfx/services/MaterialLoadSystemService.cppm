
export module Gfx.MaterialLoadSystemService;

export import Gfx.MaterialLoadSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct MaterialLoadSystemService
		: kgr::single_service<MaterialLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
