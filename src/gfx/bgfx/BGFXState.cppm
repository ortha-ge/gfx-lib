module;

#include <cstdint>
#include <optional>

export module Ortha.Gfx.BGFX.BGFXState;

import Ortha.Gfx.RenderState;

export namespace Ortha::Gfx::BGFX {

	uint64_t getBGFXPrimitiveTypeState(const PrimitiveType& primitiveType);
	uint64_t getBGFXDepthTestState(const std::optional<DepthTest>& depthTest);
	uint64_t getBGFXBufferWriteState(const BufferWriting& bufferWriting);
	uint64_t getBGFXBlendOperandState(const BlendOperand& blendOperand);
	uint64_t getBGFXBlendOperatorState(const BlendOperator& blendOperator);
	uint64_t getBGFXBlendFuncState(const BlendOperand& src, const BlendOperator& op, const BlendOperand& dest);
	uint64_t getBGFXRenderState(const RenderState& renderState);

} // namespace Ortha::Gfx::BGFX
