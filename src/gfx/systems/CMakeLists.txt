
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GfxSystems.cppm
        ImageLoadSystem.cppm
        MaterialLoadSystem.cppm
        ShaderProgramLoadSystem.cppm
)

target_sources(gfx-lib PRIVATE
        GfxSystems.cpp
        ImageLoadSystem.cpp
        MaterialLoadSystem.cpp
        ShaderProgramLoadSystem.cpp
)