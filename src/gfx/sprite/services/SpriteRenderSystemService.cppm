
export module Gfx.SpriteRenderSystemService;

export import Gfx.SpriteRenderSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx {

	struct SpriteRenderSystemService
		: kgr::single_service<SpriteRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx
