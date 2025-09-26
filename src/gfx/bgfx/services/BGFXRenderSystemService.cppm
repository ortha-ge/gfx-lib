export module Gfx.BGFX.BGFXRenderSystemService;

export import Gfx.BGFX.BGFXRenderSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Gfx::BGFX {

	struct BGFXRenderSystemService
		: kgr::single_service<BGFXRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Gfx::BGFX
