 
cmake_minimum_required(VERSION 3.31)

find_package(bgfx CONFIG REQUIRED)
find_package(Stb REQUIRED)

set(gfx-lib-src
        src/gfx/atlas/systems/ImageAtlasLoadSystem.cpp
        src/gfx/descriptors/ShaderProgramDescriptor.cpp
        src/gfx/font/systems/FontLoadSystem.cpp
        src/gfx/font/systems/FontRenderSystem.cpp
        src/gfx/services/GfxSystemsService.cpp
        src/gfx/systems/GfxSystems.cpp
        src/gfx/systems/ImageLoadSystem.cpp
        src/gfx/systems/MaterialLoadSystem.cpp
        src/gfx/systems/ShaderProgramLoadSystem.cpp
        src/gfx/systems/SpriteAnimationSystem.cpp
        src/gfx/systems/SpriteLoadSystem.cpp
        src/gfx/systems/SpriteRenderSystem.cpp
        src/gfx/tilemap/systems/TilemapLoadSystem.cpp
        src/gfx/tilemap/systems/TilemapRenderSystem.cpp
)

set(gfx-lib-modules
        src/gfx/Camera.cppm
        src/gfx/Image.cppm
        src/gfx/IndexBuffer.cppm
        src/gfx/Material.cppm
        src/gfx/Projection.cppm
        src/gfx/RenderCommand.cppm
        src/gfx/RenderObject.cppm
        src/gfx/RenderState.cppm
        src/gfx/RenderTexture.cppm
        src/gfx/ShaderPlatformInfo.cppm
        src/gfx/ShaderProgram.cppm
        src/gfx/Sprite.cppm
        src/gfx/SpriteObject.cppm
        src/gfx/TextureCoordinates.cppm
        src/gfx/VertexBuffer.cppm
        src/gfx/Viewport.cppm
        src/gfx/atlas/ImageAtlas.cppm
        src/gfx/atlas/ImageAtlasDescriptor.cppm
        src/gfx/atlas/services/ImageAtlasLoadSystemService.cppm
        src/gfx/atlas/systems/ImageAtlasLoadSystem.cppm
        src/gfx/descriptors/CameraDescriptor.cppm
        src/gfx/descriptors/ImageDescriptor.cppm
        src/gfx/descriptors/ViewportDescriptor.cppm
        src/gfx/descriptors/MaterialDescriptor.cppm
        src/gfx/descriptors/ShaderDescriptor.cppm
        src/gfx/descriptors/ShaderProgramDescriptor.cppm
        src/gfx/descriptors/SpriteDescriptor.cppm
        src/gfx/font/Font.cppm
        src/gfx/font/FontDescriptor.cppm
        src/gfx/font/FontObject.cppm
        src/gfx/font/services/FontLoadSystemService.cppm
        src/gfx/font/services/FontRenderSystemService.cppm
        src/gfx/font/systems/FontLoadSystem.cppm
        src/gfx/font/systems/FontRenderSystem.cppm
        src/gfx/services/GfxSystemsService.cppm
        src/gfx/services/ImageLoadSystemService.cppm
        src/gfx/services/MaterialLoadSystemService.cppm
        src/gfx/services/ShaderProgramLoadSystemService.cppm
        src/gfx/services/SpriteAnimationSystemService.cppm
        src/gfx/services/SpriteLoadSystemService.cppm
        src/gfx/services/SpriteRenderSystemService.cppm
        src/gfx/systems/GfxSystems.cppm
        src/gfx/systems/ImageLoadSystem.cppm
        src/gfx/systems/MaterialLoadSystem.cppm
        src/gfx/systems/ShaderProgramLoadSystem.cppm
        src/gfx/systems/SpriteAnimationSystem.cppm
        src/gfx/systems/SpriteLoadSystem.cppm
        src/gfx/systems/SpriteRenderSystem.cppm
        src/gfx/tilemap/Tilemap.cppm
        src/gfx/tilemap/TilemapDescriptor.cppm
        src/gfx/tilemap/TilemapObject.cppm
        src/gfx/tilemap/services/TilemapLoadSystemService.cppm
        src/gfx/tilemap/services/TilemapRenderSystemService.cppm
        src/gfx/tilemap/systems/TilemapLoadSystem.cppm
        src/gfx/tilemap/systems/TilemapRenderSystem.cppm
)

set(gfx-lib-reflection-src
        src/gfx-reflection/Reflection.cpp
)

set(gfx-lib-reflection-modules
        src/gfx-reflection/Camera.cppm
        src/gfx-reflection/Image.cppm
        src/gfx-reflection/Material.cppm
        src/gfx-reflection/Reflection.cppm
        src/gfx-reflection/RenderObject.cppm
        src/gfx-reflection/Projection.cppm
        src/gfx-reflection/ShaderProgram.cppm
        src/gfx-reflection/Sprite.cppm
        src/gfx-reflection/SpriteObject.cppm
        src/gfx-reflection/TextureCoordinates.cppm
        src/gfx-reflection/Viewport.cppm
        src/gfx-reflection/atlas/ImageAtlas.cppm
        src/gfx-reflection/font/FontObject.cppm
        src/gfx-reflection/tilemap/Tilemap.cppm
        src/gfx-reflection/tilemap/TilemapObject.cppm
)

add_library(gfx-lib STATIC ${gfx-lib-src} ${gfx-lib-modules} ${gfx-lib-reflection-src} ${gfx-lib-reflection-modules})

target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${gfx-lib-modules} ${gfx-lib-reflection-modules})
target_compile_features(gfx-lib PUBLIC cxx_std_23)
target_link_libraries(gfx-lib PUBLIC core-lib bgfx::bx bgfx::bimg_decode)
target_include_directories(gfx-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src PRIVATE ${Stb_INCLUDE_DIR})

add_subdirectory(tests)
add_subdirectory(src/gfx/bgfx)