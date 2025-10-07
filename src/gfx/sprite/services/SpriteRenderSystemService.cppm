
export module Ortha.Gfx.SpriteRenderSystemService;

export import Ortha.Gfx.SpriteRenderSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx {

	struct SpriteRenderSystemService
		: kgr::single_service<SpriteRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx
