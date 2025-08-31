module;

export module Gfx.Systems;

import Core.EnTTRegistry;
import Core.Scheduler;
import Gfx.BGFXSystem;
import Gfx.MaterialLoadSystem;
import Gfx.ShaderProgramLoadSystem;

export namespace Gfx {

    class GfxSystems {
    public:

        GfxSystems(Core::EnTTRegistry&, Core::Scheduler&);
        ~GfxSystems();

        GfxSystems(GfxSystems&&) = delete;
        GfxSystems& operator=(GfxSystems&&) noexcept = delete;

        GfxSystems(const GfxSystems&) = delete;
        GfxSystems& operator=(const GfxSystems&) = delete;

    private:

        Core::EnTTRegistry& mRegistry;
        Core::Scheduler& mScheduler;
        Core::TaskHandle mTickHandle{};
        BGFXSystem mBGFXSystem{};
        ShaderProgramLoadSystem mShaderProgramLoadSystem{};
        MaterialLoadSystem mMaterialLoadSystem{};

    };

} // Gfx
