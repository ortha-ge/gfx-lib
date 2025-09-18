
export module Gfx.ShaderProgramLoadSystemService;

export import Gfx.ShaderProgramLoadSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct ShaderProgramLoadSystemService
		: kgr::single_service<ShaderProgramLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
