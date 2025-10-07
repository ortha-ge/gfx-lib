module Ortha.Gfx.BGFX.SystemsService;

import Ortha.Gfx.BGFX.BGFXRenderSystemService;
import Ortha.Gfx.BGFX.BGFXResourceCreationSystemService;

namespace Ortha::Gfx::BGFX {

	void initBGFXSubSystems(BGFXSystems&, kgr::container& container) {
		container.emplace<BGFXResourceCreationSystemService>();
		container.emplace<BGFXRenderSystemService>();
	}

} // namespace Ortha::Gfx::BGFX