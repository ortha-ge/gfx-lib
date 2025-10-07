
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        BGFXContext.cppm
        BGFXFrameBuffer.cppm
        BGFXShader.cppm
        BGFXShaderProgram.cppm
        BGFXState.cppm
        BGFXTexture.cppm
        BGFXTransientIndexBuffer.cppm
        BGFXTransientVertexBuffer.cppm
        BGFXUniform.cppm
        BGFXVertexLayout.cppm
)

target_sources(ortha-gfx PRIVATE
        BGFXState.cpp
)

target_link_libraries(ortha-gfx PUBLIC bgfx::bgfx)

add_subdirectory(services)
add_subdirectory(systems)