module;

#include <utility>

#include <entt/entt.hpp>

module Gfx.Systems;

import Core.ReflectionContext;

namespace Gfx {

    GfxSystems::GfxSystems(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
        : mRegistry{ registry }
        , mScheduler{ scheduler }
		, mBGFXSystem{ registry } {

        mBGFXSystem.initSystem(mRegistry);
        mTickHandle = mScheduler.schedule([this]{
            mMaterialLoadSystem.tickSystem(mRegistry);
            mShaderProgramLoadSystem.tickSystem(mRegistry);
            mBGFXSystem.tickSystem(mRegistry);
        });
    }

    GfxSystems::~GfxSystems() {
        mScheduler.unschedule(std::move(mTickHandle));
        mBGFXSystem.destroySystem(mRegistry);
    }

} // Gfx
