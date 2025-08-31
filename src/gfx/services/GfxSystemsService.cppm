
export module Gfx.SystemsService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
export import Gfx.Systems;

export namespace Gfx {

    class GfxSystemsService : public kgr::single_service<GfxSystems, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>{};

} // Gfx
