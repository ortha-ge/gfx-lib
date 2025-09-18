 
cmake_minimum_required(VERSION 3.31)

find_package(bgfx CONFIG REQUIRED)

set(gfx-lib-src
        src/gfx/descriptors/ShaderProgramDescriptor.cpp
        src/gfx/services/GfxSystemsService.cpp
        src/gfx/systems/BGFXSystem.cpp
        src/gfx/systems/GfxSystems.cpp
        src/gfx/systems/ImageLoadSystem.cpp
        src/gfx/systems/MaterialLoadSystem.cpp
        src/gfx/systems/ShaderProgramLoadSystem.cpp
        src/gfx/systems/SpriteAnimationSystem.cpp
        src/gfx/systems/SpriteLoadSystem.cpp
        src/gfx/systems/SpriteRenderSystem.cpp
)

set(gfx-lib-modules
        src/gfx/BGFXContext.cppm
        src/gfx/BGFXDrawCallback.cppm
        src/gfx/Camera.cppm
        src/gfx/Image.cppm
        src/gfx/IndexBuffer.cppm
        src/gfx/Material.cppm
        src/gfx/Projection.cppm
        src/gfx/RenderCommand.cppm
        src/gfx/RenderObject.cppm
        src/gfx/ShaderPlatformInfo.cppm
        src/gfx/ShaderProgram.cppm
        src/gfx/Sprite.cppm
        src/gfx/SpriteObject.cppm
        src/gfx/TextureCoordinates.cppm
        src/gfx/VertexBuffer.cppm
        src/gfx/Viewport.cppm
        src/gfx/descriptors/CameraDescriptor.cppm
        src/gfx/descriptors/ImageDescriptor.cppm
        src/gfx/descriptors/ViewportDescriptor.cppm
        src/gfx/descriptors/MaterialDescriptor.cppm
        src/gfx/descriptors/ShaderDescriptor.cppm
        src/gfx/descriptors/ShaderProgramDescriptor.cppm
        src/gfx/descriptors/SpriteDescriptor.cppm
        src/gfx/services/BGFXSystemService.cppm
        src/gfx/services/GfxSystemsService.cppm
        src/gfx/services/ImageLoadSystemService.cppm
        src/gfx/services/MaterialLoadSystemService.cppm
        src/gfx/services/ShaderProgramLoadSystemService.cppm
        src/gfx/services/SpriteAnimationSystemService.cppm
        src/gfx/services/SpriteLoadSystemService.cppm
        src/gfx/services/SpriteRenderSystemService.cppm
        src/gfx/systems/BGFXSystem.cppm
        src/gfx/systems/GfxSystems.cppm
        src/gfx/systems/ImageLoadSystem.cppm
        src/gfx/systems/MaterialLoadSystem.cppm
        src/gfx/systems/ShaderProgramLoadSystem.cppm
        src/gfx/systems/SpriteAnimationSystem.cppm
        src/gfx/systems/SpriteLoadSystem.cppm
        src/gfx/systems/SpriteRenderSystem.cppm
)

set(gfx-lib-reflection-modules
        src/gfx-reflection/Camera.cppm
        src/gfx-reflection/Material.cppm
        src/gfx-reflection/Projection.cppm
        src/gfx-reflection/ShaderProgram.cppm
        src/gfx-reflection/Sprite.cppm
        src/gfx-reflection/TextureCoordinates.cppm
        src/gfx-reflection/Viewport.cppm
)

add_library(gfx-lib STATIC ${gfx-lib-src} ${gfx-lib-modules} ${gfx-lib-reflection-modules})

target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${gfx-lib-modules} ${gfx-lib-reflection-modules})
target_compile_features(gfx-lib PUBLIC cxx_std_23)
target_link_libraries(gfx-lib PUBLIC core-lib bgfx::bx bgfx::bimg_decode bgfx::bgfx)
target_include_directories(gfx-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

#add_subdirectory(tests)
