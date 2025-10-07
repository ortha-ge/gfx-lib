
export module Ortha.Gfx.MaterialLoadSystemService;

export import Ortha.Gfx.MaterialLoadSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct MaterialLoadSystemService
		: kgr::single_service<MaterialLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
