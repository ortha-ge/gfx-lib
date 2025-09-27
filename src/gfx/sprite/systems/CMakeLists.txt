
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        SpriteAnimationSystem.cppm
        SpriteLoadSystem.cppm
        SpriteRenderSystem.cppm
)

target_sources(gfx-lib PRIVATE
        SpriteAnimationSystem.cpp
        SpriteLoadSystem.cpp
        SpriteRenderSystem.cpp
)
