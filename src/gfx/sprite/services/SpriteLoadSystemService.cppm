
export module Gfx.SpriteLoadSystemService;

export import Gfx.SpriteLoadSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct SpriteLoadSystemService
		: kgr::single_service<SpriteLoadSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
