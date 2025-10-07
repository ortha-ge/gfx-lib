
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CreateRenderCandidatesVisitorSystem.cppm
)

target_sources(ortha-gfx PRIVATE
        CreateRenderCandidatesVisitorSystem.cpp
)
