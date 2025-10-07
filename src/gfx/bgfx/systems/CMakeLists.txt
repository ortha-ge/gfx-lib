
target_sources(ortha-gfx
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        BGFXRenderSystem.cppm
        BGFXResourceCreationSystem.cppm
        BGFXSystems.cppm
)

target_sources(ortha-gfx PRIVATE
        BGFXRenderSystem.cpp
        BGFXResourceCreationSystem.cpp
        BGFXSystems.cpp
)
