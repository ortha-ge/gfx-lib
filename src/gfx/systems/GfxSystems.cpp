module;

#include <utility>

#include <entt/entt.hpp>

module Gfx.Systems;

import Core.ReflectionContext;
import Gfx.Reflection.Colour;
import Gfx.Reflection.MaterialDescriptor;
import Gfx.Reflection.ShaderProgramDescriptor;
import Gfx.Reflection.TextureCoordinates;

namespace Gfx {

    GfxSystems::GfxSystems(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
        : mRegistry{ registry }
        , mScheduler{ scheduler }
		, mBGFXSystem{ registry } {

        Core::reflect<Colour>(Core::getCurrentReflectionContext());
        Core::reflect<TextureCoordinates>(Core::getCurrentReflectionContext());
        Core::reflect<ShaderProgramDescriptor>(Core::getCurrentReflectionContext());
        Core::reflect<MaterialDescriptor>(Core::getCurrentReflectionContext());

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
