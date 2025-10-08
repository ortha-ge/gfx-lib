module Ortha.Gfx.SystemsService;

import Ortha.RTTI.ReflectionContextStack;
import Ortha.Gfx.CreateRenderCandidatesVisitorSystemService;
import Ortha.Gfx.FontLoadSystemService;
import Ortha.Gfx.FontRenderSystemService;
import Ortha.Gfx.ImageAtlasLoadSystemService;
import Ortha.Gfx.ImageLoadSystemService;
import Ortha.Gfx.MaterialLoadSystemService;
import Ortha.Gfx.ShaderProgramLoadSystemService;
import Ortha.Gfx.SpriteAnimationSystemService;
import Ortha.Gfx.SpriteLoadSystemService;
import Ortha.Gfx.SpriteRenderSystemService;
import Ortha.Gfx.TilemapLoadSystemService;
import Ortha.Gfx.TilemapRenderSystemService;
import Ortha.Gfx.Reflection;
import Ortha.Gfx.BGFX.SystemsService;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::Gfx {

	void initGfxSubSystems(GfxSystems&, kgr::container& container) {
		using namespace RTTI;

		reflectAll(ReflectionContextStack::getCurrentContext());

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

} // namespace Ortha::Gfx
