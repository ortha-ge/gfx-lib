module Gfx.SystemsService;

import Ortha.RTTI.ReflectionContextStack;
import Gfx.CreateRenderCandidatesVisitorSystemService;
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
import Gfx.BGFX.SystemsService;
import Ortha.RTTI.ReflectionContext;

namespace Gfx {

	void initGfxSubSystems(GfxSystems&, kgr::container& container) {
		reflectAll(static_cast<Ortha::RTTI::ReflectionContext&>(Ortha::RTTI::getCurrentReflectionContext()));

		// Camera + RenderCandidates
		container.emplace<CreateRenderCandidatesVisitorSystemService>();

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

		container.emplace<BGFX::SystemsService>();
	}

} // namespace Gfx
