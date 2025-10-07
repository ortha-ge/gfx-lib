
#include <cstdint>

#include <bgfx/bgfx.h>
#include <catch2/catch_test_macros.hpp>

import Ortha.Gfx.RenderState;
import Ortha.Gfx.BGFX.BGFXState;

TEST_CASE("SpriteRenderState_GetBGFXState_IsExpected", "BGFXState") {
	using namespace Ortha::Gfx;
	using namespace Ortha::Gfx::BGFX;

	constexpr uint64_t expectedState = 0
		| BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
		| BGFX_STATE_BLEND_EQUATION_ADD | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
	RenderState renderState{};
	renderState.bufferWriting = BufferWriting::RGB | BufferWriting::Alpha;
	renderState.blendLhs = BlendOperand::SourceAlpha;
	renderState.blendOperator = BlendOperator::Add;
	renderState.blendRhs = BlendOperand::InverseSourceAlpha;

	const uint64_t bgfxState = getBGFXRenderState(renderState);

	REQUIRE(bgfxState == expectedState);
}