
target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ImageDescriptor.cppm
        MaterialDescriptor.cppm
        ShaderDescriptor.cppm
        ShaderProgramDescriptor.cppm
        ViewportDescriptor.cppm
)

target_sources(gfx-lib PRIVATE
        ShaderProgramDescriptor.cpp
)
