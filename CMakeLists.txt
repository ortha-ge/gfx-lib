 
cmake_minimum_required(VERSION 3.31)

set (gfx-lib-src
        src/gfx/systems/BGFXSystem.cpp
        src/gfx/systems/GfxSystems.cpp
        src/gfx/resources/systems/MaterialLoadSystem.cpp
        src/gfx/resources/systems/ShaderProgramLoadSystem.cpp
)

set (gfx-lib-modules
        src/gfx/BGFXContext.cppm
        src/gfx/BGFXDrawCallback.cppm
        src/gfx/Colour.cppm
        src/gfx/RenderObject.cppm
        src/gfx/TextureCoordinates.cppm
        src/gfx/resources/MaterialResource.cppm
        src/gfx/resources/ShaderProgramResource.cppm
        src/gfx/resources/descriptors/MaterialDescriptor.cppm
        src/gfx/resources/descriptors/ShaderDescriptor.cppm
        src/gfx/resources/descriptors/ShaderProgramDescriptor.cppm
        src/gfx/services/GfxSystemsService.cppm
        src/gfx/systems/BGFXSystem.cppm
        src/gfx/systems/GfxSystems.cppm
        src/gfx/resources/systems/MaterialLoadSystem.cppm
        src/gfx/resources/systems/ShaderProgramLoadSystem.cppm
        src/gfx-reflection/ColourReflection.cppm
        src/gfx-reflection/MaterialDescriptorReflection.cppm
        src/gfx-reflection/ShaderProgramDescriptorReflection.cppm
        src/gfx-reflection/TextureCoordinatesReflection.cppm
)

add_library(gfx-lib STATIC ${gfx-lib-src} ${gfx-lib-modules})

target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${gfx-lib-modules})
target_compile_features(gfx-lib PUBLIC cxx_std_23)
target_link_libraries(gfx-lib PUBLIC core-lib bx bimg_decode bgfx)
target_include_directories(gfx-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

#add_subdirectory(tests)
