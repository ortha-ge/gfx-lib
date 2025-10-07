
target_sources(ortha-gfx
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        BGFXRenderSystemService.cppm
        BGFXResourceCreationSystemService.cppm
        BGFXSystemsService.cppm
)

target_sources(ortha-gfx PRIVATE
        BGFXSystemsService.cpp
)
