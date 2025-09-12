module;

export module Gfx.Reflection.MaterialDescriptor;

export import Gfx.MaterialDescriptor;

import Core.ReflectionContext;
import Glm.Reflection.Vec4;

template<>
void Core::reflect<Gfx::MaterialDescriptor>(ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<MaterialDescriptor>("Material")
		.property("shaderProgramFilePath", &MaterialDescriptor::shaderProgramFilePath)
		.property("textureImageFilePath", &MaterialDescriptor::textureImageFilePath)
		.property("alphaColour", &MaterialDescriptor::alphaColour)
		.build();
}
