
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ImageAtlasLoadSystem.cppm
)

target_sources(gfx-lib PRIVATE
        ImageAtlasLoadSystem.cpp
)
