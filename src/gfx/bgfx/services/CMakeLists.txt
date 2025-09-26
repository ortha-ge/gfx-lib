
target_sources(gfx-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        BGFXRenderSystemService.cppm
        BGFXResourceCreationSystemService.cppm
        BGFXSystemsService.cppm
)

target_sources(gfx-lib PRIVATE
        BGFXSystemsService.cpp
)
