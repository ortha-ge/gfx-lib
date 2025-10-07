module;

export module Ortha.Gfx.Reflection.MaterialDescriptor;

export import Ortha.Gfx.MaterialDescriptor;

import Ortha.Core.Reflect;
import Glm.Reflection.Vec4;

namespace Ortha::Core {

	template<>
	void reflect<Gfx::MaterialDescriptor>(RTTI::ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<MaterialDescriptor>("Material")
			.field<&MaterialDescriptor::shaderProgramFilePath>("shaderProgramFilePath")
			.field<&MaterialDescriptor::textureImageFilePath>("textureImageFilePath")
			.field<&MaterialDescriptor::alphaColour>("alphaColour")
			;
	}

}