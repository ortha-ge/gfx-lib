
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ImageAtlasLoadSystem.cppm
)

target_sources(ortha-gfx PRIVATE
        ImageAtlasLoadSystem.cpp
)
