
export module Ortha.Gfx.SystemsService;

export import Ortha.Gfx.Systems;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	void initGfxSubSystems(GfxSystems&, kgr::container&);

	struct GfxSystemsService
		: kgr::single_service<GfxSystems>
		, kgr::autocall<kgr::invoke_method<initGfxSubSystems>>{};

} // namespace Ortha::Gfx
