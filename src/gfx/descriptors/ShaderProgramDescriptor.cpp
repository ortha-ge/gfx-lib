module;

#include <cstddef>
#include <vector>

module Gfx.ShaderProgramDescriptor;

namespace Gfx {

	size_t getBufferSizeForVertexLayout(const ShaderVertexLayoutDescriptor& vertexLayout, size_t vertexCount) {
		return getStrideForVertexLayout(vertexLayout) * vertexCount;
	}

	size_t getStrideForVertexLayout(const ShaderVertexLayoutDescriptor& vertexLayout) {
		size_t stride{ 0zu };
		for (auto&& attribute : vertexLayout.attributes) {
			stride += attribute.typeId.getTypeInfo().getSize() * attribute.count;
		}

		return stride;
	}

	size_t getOffsetForVertexLayoutAttribute(const ShaderVertexLayoutDescriptor& vertexLayout, ShaderVertexLayoutAttributeId attributeId) {
		size_t offset{ 0zu };
		for (auto&& attribute : vertexLayout.attributes) {
			if (attribute.attributeId == attributeId) {
				break;
			}

			offset += attribute.typeId.getTypeInfo().getSize() * attribute.count;
		}

		return offset;
	}

} // namespace Gfx
