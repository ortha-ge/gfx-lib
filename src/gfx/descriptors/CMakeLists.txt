
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ImageDescriptor.cppm
        MaterialDescriptor.cppm
        ShaderDescriptor.cppm
        ShaderProgramDescriptor.cppm
        ViewportDescriptor.cppm
)

target_sources(ortha-gfx PRIVATE
        ShaderProgramDescriptor.cpp
)
