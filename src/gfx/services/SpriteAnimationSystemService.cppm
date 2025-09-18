
export module Gfx.SpriteAnimationSystemService;

export import Gfx.SpriteAnimationSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct SpriteAnimationSystemService
		: kgr::single_service<SpriteAnimationSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
