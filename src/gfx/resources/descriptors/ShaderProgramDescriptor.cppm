module;

#include <string>

export module Gfx.ShaderProgramDescriptor;

export namespace Gfx {

    struct ShaderProgramDescriptor {
        std::string vertexShaderFilePath;
        std::string fragmentShaderFilePath;
    };

} // Gfx
