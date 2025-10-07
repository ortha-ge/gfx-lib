
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        FontLoadSystem.cppm
        FontRenderSystem.cppm
)

target_sources(ortha-gfx PRIVATE
        FontLoadSystem.cpp
        FontRenderSystem.cpp
)
