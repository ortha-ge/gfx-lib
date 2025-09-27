
target_sources(gfx-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        Camera.cppm
        CameraDescriptor.cppm
        RenderCandidates.cppm
        services/CreateRenderCandidatesVisitorSystemService.cppm
        systems/CreateRenderCandidatesVisitorSystem.cppm
)

target_sources(gfx-lib
        PRIVATE
        systems/CreateRenderCandidatesVisitorSystem.cpp
)
