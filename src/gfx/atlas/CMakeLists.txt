
target_sources(gfx-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        ImageAtlas.cppm
        ImageAtlasDescriptor.cppm
        services/ImageAtlasLoadSystemService.cppm
        systems/ImageAtlasLoadSystem.cppm
)

target_sources(gfx-lib
        PRIVATE
        systems/ImageAtlasLoadSystem.cpp
)
