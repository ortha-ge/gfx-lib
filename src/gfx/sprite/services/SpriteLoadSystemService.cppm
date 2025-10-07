
export module Ortha.Gfx.SpriteLoadSystemService;

export import Ortha.Gfx.SpriteLoadSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct SpriteLoadSystemService
		: kgr::single_service<SpriteLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
