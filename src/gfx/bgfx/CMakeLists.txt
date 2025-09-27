
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
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

target_sources(gfx-lib PRIVATE
        BGFXState.cpp
)

target_link_libraries(gfx-lib PUBLIC bgfx::bgfx)

add_subdirectory(services)
add_subdirectory(systems)