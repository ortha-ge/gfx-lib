
target_sources(gfx-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        Tilemap.cppm
        TilemapDescriptor.cppm
        TilemapObject.cppm
        services/TilemapLoadSystemService.cppm
        services/TilemapRenderSystemService.cppm
        systems/TilemapLoadSystem.cppm
        systems/TilemapRenderSystem.cppm
)

target_sources(gfx-lib
        PRIVATE
        systems/TilemapLoadSystem.cpp
        systems/TilemapRenderSystem.cpp
)
