
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        TilemapLoadSystem.cppm
        TilemapRenderSystem.cppm
)

target_sources(gfx-lib PRIVATE
        TilemapLoadSystem.cpp
        TilemapRenderSystem.cpp
)
