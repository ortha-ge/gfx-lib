
target_sources(gfx-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        GfxSystemsService.cppm
        ImageLoadSystemService.cppm
        MaterialLoadSystemService.cppm
        ShaderProgramLoadSystemService.cppm
)

target_sources(gfx-lib
        PRIVATE
        GfxSystemsService.cpp
)