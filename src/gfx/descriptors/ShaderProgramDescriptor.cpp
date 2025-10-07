module;

#include <cstddef>
#include <vector>

module Ortha.Gfx.ShaderProgramDescriptor;

import Ortha.RTTI.TypeInfo;

namespace Ortha::Gfx {

	size_t getBufferSizeForVertexLayout(const ShaderVertexLayoutDescriptor& vertexLayout, size_t vertexCount) {
		return getStrideForVertexLayout(vertexLayout) * vertexCount;
	}

	size_t getStrideForVertexLayout(const ShaderVertexLayoutDescriptor& vertexLayout) {
		using namespace Ortha::RTTI;
		size_t stride{ 0zu };
		for (auto&& attribute : vertexLayout.attributes) {
			stride += getTypeInfo(attribute.typeId).getSize() * attribute.count;
		}

		return stride;
	}

	size_t getOffsetForVertexLayoutAttribute(const ShaderVertexLayoutDescriptor& vertexLayout, ShaderVertexLayoutAttributeId attributeId) {
		size_t offset{ 0zu };
		for (auto&& attribute : vertexLayout.attributes) {
			if (attribute.attributeId == attributeId) {
				break;
			}

			offset += getTypeInfo(attribute.typeId).getSize() * attribute.count;
		}

		return offset;
	}

} // namespace Ortha::Gfx
