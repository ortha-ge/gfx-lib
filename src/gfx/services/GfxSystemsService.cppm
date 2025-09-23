
export module Gfx.SystemsService;

export import Gfx.Systems;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	void initGfxSubSystems(GfxSystems&, kgr::container&);

	struct GfxSystemsService
		: kgr::single_service<GfxSystems>
		, kgr::autocall<kgr::invoke_method<initGfxSubSystems>>{};

} // namespace Gfx
