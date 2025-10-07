
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Camera.cppm
        Image.cppm
        Material.cppm
        Reflection.cppm
        RenderObject.cppm
        Projection.cppm
        ShaderProgram.cppm
        Sprite.cppm
        SpriteObject.cppm
        TextureCoordinates.cppm
        Viewport.cppm
)

target_sources(ortha-gfx PRIVATE
        Reflection.cpp
)

add_subdirectory(atlas)
add_subdirectory(font)
add_subdirectory(tilemap)
