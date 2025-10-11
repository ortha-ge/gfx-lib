module;

export module Ortha.Gfx.Reflection.MaterialDescriptor;

export import Ortha.Gfx.MaterialDescriptor;

import Glm.Reflection.Vec4;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template<>
	void reflect<Gfx::MaterialDescriptor>(ReflectionContext& reflectionContext) {
		using namespace Gfx;
		reflectionContext.addClass<MaterialDescriptor>("Material")
			.field<&MaterialDescriptor::shaderProgramFilePath>("shaderProgramFilePath")
			.field<&MaterialDescriptor::textureImageFilePath>("textureImageFilePath")
			.field<&MaterialDescriptor::alphaColour>("alphaColour");
	}

}