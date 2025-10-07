
target_sources(ortha-gfx
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        GfxSystemsService.cppm
        ImageLoadSystemService.cppm
        MaterialLoadSystemService.cppm
        ShaderProgramLoadSystemService.cppm
)

target_sources(ortha-gfx
        PRIVATE
        GfxSystemsService.cpp
)