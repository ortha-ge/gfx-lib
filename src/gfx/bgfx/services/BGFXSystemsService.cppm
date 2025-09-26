
export module Gfx.BGFX.SystemsService;

export import Gfx.BGFX.Systems;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx::BGFX {

	void initBGFXSubSystems(BGFXSystems&, kgr::container&);

	struct SystemsService
		: kgr::single_service<BGFXSystems, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>,
		  kgr::autocall<kgr::invoke_method<initBGFXSubSystems>> {};

} // namespace Gfx::BGFX
