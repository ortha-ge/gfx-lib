
export module Ortha.Gfx.BGFX.SystemsService;

export import Ortha.Gfx.BGFX.Systems;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx::BGFX {

	void initBGFXSubSystems(BGFXSystems&, kgr::container&);

	struct SystemsService
		: kgr::single_service<BGFXSystems, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>,
		  kgr::autocall<kgr::invoke_method<initBGFXSubSystems>> {};

} // namespace Ortha::Gfx::BGFX
