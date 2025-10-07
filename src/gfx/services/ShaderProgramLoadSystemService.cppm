
export module Ortha.Gfx.ShaderProgramLoadSystemService;

export import Ortha.Gfx.ShaderProgramLoadSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct ShaderProgramLoadSystemService
		: kgr::single_service<ShaderProgramLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
