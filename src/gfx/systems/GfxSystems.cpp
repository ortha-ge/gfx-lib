module;

#include <utility>

#include <entt/entt.hpp>

module Gfx.Systems;

import Core.ReflectionContext;

namespace Gfx {

	GfxSystems::GfxSystems(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mBGFXSystem{ registry, scheduler }
		, mImageLoadSystem(registry, scheduler)
		, mShaderProgramLoadSystem(registry, scheduler)
		, mMaterialLoadSystem(registry, scheduler)
		, mSpriteAnimationSystem(registry, scheduler)
		, mSpriteLoadSystem(registry, scheduler)
		, mSpriteRenderSystem(registry, scheduler) {}

} // namespace Gfx
