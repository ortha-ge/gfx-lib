
export module Ortha.Gfx.SpriteAnimationSystemService;

export import Ortha.Gfx.SpriteAnimationSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct SpriteAnimationSystemService
		: kgr::single_service<SpriteAnimationSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
