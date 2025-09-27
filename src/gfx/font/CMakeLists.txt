
target_sources(gfx-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        Font.cppm
        FontDescriptor.cppm
        FontObject.cppm
        services/FontLoadSystemService.cppm
        services/FontRenderSystemService.cppm
        systems/FontLoadSystem.cppm
        systems/FontRenderSystem.cppm
)

target_sources(gfx-lib
        PRIVATE
        systems/FontLoadSystem.cpp
        systems/FontRenderSystem.cpp
)
