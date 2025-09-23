module;

#include <entt/entt.hpp>

module Gfx.SystemsService;

import Core.ReflectionContext;
import Gfx.BGFXSystemService;
import Gfx.FontLoadSystemService;
import Gfx.FontRenderSystemService;
import Gfx.ImageAtlasLoadSystemService;
import Gfx.ImageLoadSystemService;
import Gfx.MaterialLoadSystemService;
import Gfx.ShaderProgramLoadSystemService;
import Gfx.SpriteAnimationSystemService;
import Gfx.SpriteLoadSystemService;
import Gfx.SpriteRenderSystemService;
import Gfx.TilemapLoadSystemService;
import Gfx.TilemapRenderSystemService;
import Gfx.Reflection;

namespace Gfx {

	void initGfxSubSystems(GfxSystems&, kgr::container& container) {
		reflectAll(Core::getCurrentReflectionContext());

		// Resource
		container.emplace<SpriteLoadSystemService>();
		container.emplace<MaterialLoadSystemService>();
		container.emplace<ShaderProgramLoadSystemService>();
		container.emplace<ImageLoadSystemService>();
		container.emplace<ImageAtlasLoadSystemService>();

		// Font
		container.emplace<FontLoadSystemService>();
		container.emplace<FontRenderSystemService>();

		// Tilemap
		container.emplace<TilemapLoadSystemService>();
		container.emplace<TilemapRenderSystemService>();

		// Sprite
		container.emplace<SpriteAnimationSystemService>();
		container.emplace<SpriteRenderSystemService>();

		container.emplace<BGFXSystemService>();
	}

} // namespace Gfx
