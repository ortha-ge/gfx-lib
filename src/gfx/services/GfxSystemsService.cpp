module;

#include <entt/entt.hpp>

module Gfx.SystemsService;

import Gfx.BGFXSystemService;
import Gfx.ImageLoadSystemService;
import Gfx.MaterialLoadSystemService;
import Gfx.ShaderProgramLoadSystemService;
import Gfx.SpriteAnimationSystemService;
import Gfx.SpriteLoadSystemService;
import Gfx.SpriteRenderSystemService;

namespace Gfx {

	void initGfxSubSystems(GfxSystems&, kgr::container& container) {
		// Resource
		container.emplace<SpriteLoadSystemService>();
		container.emplace<MaterialLoadSystemService>();
		container.emplace<ShaderProgramLoadSystemService>();
		container.emplace<ImageLoadSystemService>();

		// Sprite
		container.emplace<SpriteAnimationSystemService>();
		container.emplace<SpriteRenderSystemService>();

		container.emplace<BGFXSystemService>();
	}

} // namespace Gfx
