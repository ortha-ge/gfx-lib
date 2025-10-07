
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Sprite.cppm
        SpriteDescriptor.cppm
        SpriteObject.cppm
)

target_sources(ortha-gfx PRIVATE
)

add_subdirectory(services)
add_subdirectory(systems)
