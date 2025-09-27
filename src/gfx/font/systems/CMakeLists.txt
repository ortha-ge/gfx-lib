
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        FontLoadSystem.cppm
        FontRenderSystem.cppm
)

target_sources(gfx-lib PRIVATE
        FontLoadSystem.cpp
        FontRenderSystem.cpp
)
