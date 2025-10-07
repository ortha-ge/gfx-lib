module;

export module Ortha.Gfx.Reflection.MaterialDescriptor;

export import Ortha.Gfx.MaterialDescriptor;

import Ortha.Core.Reflect;
import Glm.Reflection.Vec4;

template<>
void Core::reflect<Ortha::Gfx::MaterialDescriptor>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	using namespace Ortha::Gfx;
	reflectionContext.addClass<MaterialDescriptor>("Material")
		.field<&MaterialDescriptor::shaderProgramFilePath>("shaderProgramFilePath")
		.field<&MaterialDescriptor::textureImageFilePath>("textureImageFilePath")
		.field<&MaterialDescriptor::alphaColour>("alphaColour")
		;
}
