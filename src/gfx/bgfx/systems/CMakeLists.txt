
target_sources(gfx-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        BGFXRenderSystem.cppm
        BGFXResourceCreationSystem.cppm
        BGFXSystems.cppm
)

target_sources(gfx-lib PRIVATE
        BGFXRenderSystem.cpp
        BGFXResourceCreationSystem.cpp
        BGFXSystems.cpp
)
