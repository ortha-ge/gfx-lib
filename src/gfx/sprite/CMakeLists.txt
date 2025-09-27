
target_sources(gfx-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        Sprite.cppm
        SpriteObject.cppm
        services/SpriteAnimationSystemService.cppm
        services/SpriteLoadSystemService.cppm
        services/SpriteRenderSystemService.cppm
        systems/SpriteAnimationSystem.cppm
        systems/SpriteLoadSystem.cppm
        systems/SpriteRenderSystem.cppm
)

target_sources(gfx-lib
        PRIVATE
        systems/SpriteAnimationSystem.cpp
        systems/SpriteLoadSystem.cpp
        systems/SpriteRenderSystem.cpp
)
