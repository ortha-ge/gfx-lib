export module Ortha.Gfx.BGFX.BGFXRenderSystemService;

export import Ortha.Gfx.BGFX.BGFXRenderSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Gfx::BGFX {

	struct BGFXRenderSystemService
		: kgr::single_service<BGFXRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Ortha::Gfx::BGFX
