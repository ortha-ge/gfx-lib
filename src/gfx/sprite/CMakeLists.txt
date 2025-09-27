
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Sprite.cppm
        SpriteDescriptor.cppm
        SpriteObject.cppm
)

target_sources(gfx-lib PRIVATE
)

add_subdirectory(services)
add_subdirectory(systems)
