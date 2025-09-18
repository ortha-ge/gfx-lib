module;

export module Gfx.Systems;

import Core.EnTTRegistry;
import Core.Scheduler;


export namespace Gfx {

	class GfxSystems {
	public:
		GfxSystems();

		GfxSystems(GfxSystems&&) = delete;
		GfxSystems& operator=(GfxSystems&&) noexcept = delete;

		GfxSystems(const GfxSystems&) = delete;
		GfxSystems& operator=(const GfxSystems&) = delete;

	};

} // namespace Gfx
