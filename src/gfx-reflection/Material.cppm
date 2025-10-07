module;

export module Gfx.Reflection.MaterialDescriptor;

export import Gfx.MaterialDescriptor;

import Core.Reflect;
import Glm.Reflection.Vec4;

template<>
void Core::reflect<Gfx::MaterialDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Gfx;
	reflectionContext.addClass<MaterialDescriptor>("Material")
		.field<&MaterialDescriptor::shaderProgramFilePath>("shaderProgramFilePath")
		.field<&MaterialDescriptor::textureImageFilePath>("textureImageFilePath")
		.field<&MaterialDescriptor::alphaColour>("alphaColour")
		;
}
