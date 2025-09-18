
export module Gfx.SystemsService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
export import Gfx.Systems;

export namespace Gfx {

	void initGfxSubSystems(GfxSystems&, kgr::container&);

	struct GfxSystemsService
		: kgr::single_service<GfxSystems>
		, kgr::autocall<kgr::invoke_method<initGfxSubSystems>>{};

} // namespace Gfx
