
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CreateRenderCandidatesVisitorSystem.cppm
)

target_sources(gfx-lib PRIVATE
        CreateRenderCandidatesVisitorSystem.cpp
)
