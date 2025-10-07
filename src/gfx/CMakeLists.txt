
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Image.cppm
        IndexBuffer.cppm
        Material.cppm
        Projection.cppm
        RenderCommand.cppm
        RenderObject.cppm
        RenderState.cppm
        RenderTexture.cppm
        ShaderPlatformInfo.cppm
        ShaderProgram.cppm
        TextureCoordinates.cppm
        VertexBuffer.cppm
        Viewport.cppm
        ViewportUtilities.cppm
)

target_sources(ortha-gfx PRIVATE
        ViewportUtilities.cpp
)

add_subdirectory(atlas)
add_subdirectory(bgfx)
add_subdirectory(camera)
add_subdirectory(descriptors)
add_subdirectory(font)
add_subdirectory(services)
add_subdirectory(sprite)
add_subdirectory(systems)
add_subdirectory(tilemap)
