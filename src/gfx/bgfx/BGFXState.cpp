module;

#include <cstdint>
#include <optional>

#include <bgfx/bgfx.h>

module Ortha.Gfx.BGFX.BGFXState;

namespace Ortha::Gfx::BGFX {

	uint64_t getBGFXPrimitiveTypeState(const PrimitiveType& primitiveType) {
		switch (primitiveType) {
			case PrimitiveType::Triangles:
				return 0u;
			case PrimitiveType::TriangleStrip:
				return BGFX_STATE_PT_TRISTRIP;
			case PrimitiveType::Lines:
				return BGFX_STATE_PT_LINES;
			case PrimitiveType::LineStrip:
				return BGFX_STATE_PT_LINESTRIP;
			case PrimitiveType::Points:
				return BGFX_STATE_PT_POINTS;
		}
	}

	uint64_t getBGFXDepthTestState(const std::optional<DepthTest>& depthTest) {
		if (!depthTest) {
			return 0;
		}

		switch (*depthTest) {
			case DepthTest::Less:
				return BGFX_STATE_DEPTH_TEST_LESS;
			case DepthTest::LessOrEqual:
				return BGFX_STATE_DEPTH_TEST_LEQUAL;
			case DepthTest::Equal:
				return BGFX_STATE_DEPTH_TEST_EQUAL;
			case DepthTest::GreaterOrEqual:
				return BGFX_STATE_DEPTH_TEST_GEQUAL;
			case DepthTest::Greater:
				return BGFX_STATE_DEPTH_TEST_GREATER;
			case DepthTest::NotEqual:
				return BGFX_STATE_DEPTH_TEST_NOTEQUAL;
			case DepthTest::Never:
				return BGFX_STATE_DEPTH_TEST_NEVER;
			case DepthTest::Always:
				return BGFX_STATE_DEPTH_TEST_ALWAYS;
		}
	}

	uint64_t getBGFXBufferWriteState(const BufferWriting& bufferWriting) {
		uint64_t bufferWriteState = 0;
		if ((bufferWriting & BufferWriting::Red) == BufferWriting::Red) {
			bufferWriteState |= BGFX_STATE_WRITE_R;
		}

		if ((bufferWriting & BufferWriting::Green) == BufferWriting::Green) {
			bufferWriteState |= BGFX_STATE_WRITE_G;
		}

		if ((bufferWriting & BufferWriting::Blue) == BufferWriting::Blue) {
			bufferWriteState |= BGFX_STATE_WRITE_B;
		}

		if ((bufferWriting & BufferWriting::Alpha) == BufferWriting::Alpha) {
			bufferWriteState |= BGFX_STATE_WRITE_A;
		}

		if ((bufferWriting & BufferWriting::Z) == BufferWriting::Z) {
			bufferWriteState |= BGFX_STATE_WRITE_Z;
		}

		return bufferWriteState;
	}

	uint64_t getBGFXBlendOperandState(const BlendOperand& blendOperand) {
		switch (blendOperand) {
			case BlendOperand::Zero:
				return BGFX_STATE_BLEND_ZERO;
			case BlendOperand::One:
				return BGFX_STATE_BLEND_ONE;
			case BlendOperand::SourceColour:
				return BGFX_STATE_BLEND_SRC_COLOR;
			case BlendOperand::InverseSourceColour:
				return BGFX_STATE_BLEND_INV_SRC_COLOR;
			case BlendOperand::SourceAlpha:
				return BGFX_STATE_BLEND_SRC_ALPHA;
			case BlendOperand::InverseSourceAlpha:
				return BGFX_STATE_BLEND_INV_SRC_ALPHA;
			case BlendOperand::DestAlpha:
				return BGFX_STATE_BLEND_DST_ALPHA;
			case BlendOperand::InverseDestAlpha:
				return BGFX_STATE_BLEND_INV_DST_ALPHA;
			case BlendOperand::DestColour:
				return BGFX_STATE_BLEND_DST_COLOR;
			case BlendOperand::InverseDestColour:
				return BGFX_STATE_BLEND_INV_DST_COLOR;
			case BlendOperand::SourceAlphaSaturation:
				return BGFX_STATE_BLEND_SRC_ALPHA_SAT;
			case BlendOperand::Factor:
				return BGFX_STATE_BLEND_FACTOR;
			case BlendOperand::InverseFactor:
				return BGFX_STATE_BLEND_INV_FACTOR;
		}
	}

	uint64_t getBGFXBlendOperatorState(const BlendOperator& blendOperator) {
		switch (blendOperator) {
			case BlendOperator::Add:
				return BGFX_STATE_BLEND_EQUATION_ADD;
			case BlendOperator::Subtract:
				return BGFX_STATE_BLEND_EQUATION_SUB;
			case BlendOperator::InverseSubtract:
				return BGFX_STATE_BLEND_EQUATION_REVSUB;
			case BlendOperator::Min:
				return BGFX_STATE_BLEND_EQUATION_MIN;
			case BlendOperator::Max:
				return BGFX_STATE_BLEND_EQUATION_MASK;
		}
	}

	uint64_t getBGFXBlendFuncState(const BlendOperand& src, const BlendOperator& op, const BlendOperand& dest) {
		return getBGFXBlendOperatorState(op)
			| BGFX_STATE_BLEND_FUNC(getBGFXBlendOperandState(src), getBGFXBlendOperandState(dest));
	}

	uint64_t getBGFXRenderState(const RenderState& renderState) {
		uint64_t state = 0
			| getBGFXPrimitiveTypeState(renderState.primitiveType)
			| getBGFXDepthTestState(renderState.depthTest)
			| getBGFXBufferWriteState(renderState.bufferWriting)
			| getBGFXBlendFuncState(renderState.blendLhs, renderState.blendOperator, renderState.blendRhs);

		if (renderState.msaa) {
			state |= BGFX_STATE_MSAA;
		}

		return state;
	}

} // namespace Ortha::Gfx::BGFX
