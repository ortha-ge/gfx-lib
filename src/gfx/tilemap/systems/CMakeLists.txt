
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        TilemapLoadSystem.cppm
        TilemapRenderSystem.cppm
)

target_sources(ortha-gfx PRIVATE
        TilemapLoadSystem.cpp
        TilemapRenderSystem.cpp
)
