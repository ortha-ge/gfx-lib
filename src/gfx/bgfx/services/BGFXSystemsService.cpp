module;

#include <entt/entt.hpp>

module Gfx.BGFX.SystemsService;

import Gfx.BGFX.BGFXRenderSystemService;
import Gfx.BGFX.BGFXResourceCreationSystemService;

namespace Gfx::BGFX {

	void initBGFXSubSystems(BGFXSystems&, kgr::container& container) {
		container.emplace<BGFXResourceCreationSystemService>();
		container.emplace<BGFXRenderSystemService>();
	}

} // namespace Gfx::BGFX