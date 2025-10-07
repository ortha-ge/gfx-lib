
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        SpriteAnimationSystem.cppm
        SpriteLoadSystem.cppm
        SpriteRenderSystem.cppm
)

target_sources(ortha-gfx PRIVATE
        SpriteAnimationSystem.cpp
        SpriteLoadSystem.cpp
        SpriteRenderSystem.cpp
)
