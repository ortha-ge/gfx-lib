module;

#include <cstdint>
#include <optional>

#include <bgfx/bgfx.h>

export module Ortha.Gfx.RenderState;

export namespace Ortha::Gfx {

	enum class PrimitiveType : uint8_t {
		Triangles,
		TriangleStrip,
		Lines,
		LineStrip,
		Points,
	};

	enum class DepthTest : uint8_t {
		Less,
		LessOrEqual,
		Equal,
		GreaterOrEqual,
		Greater,
		NotEqual,
		Never,
		Always,
	};

	enum class BlendOperand : uint16_t {
		Zero, // 0, 0, 0, 0
		One, // 1, 1, 1, 1
		SourceColour, // Rs, Gs, Bs, As
		InverseSourceColour, // 1-Rs, 1-Gs, 1-Bs, 1-As
		SourceAlpha, // As, As, As, As
		InverseSourceAlpha, // 1-As, 1-As, 1-As, 1-As
		DestAlpha, // Ad, Ad, Ad, Ad
		InverseDestAlpha, // 1-Ad, 1-Ad, 1-Ad ,1-Ad
		DestColour, // Rd, Gd, Bd, Ad
		InverseDestColour, // -Rd, 1-Gd, 1-Bd, 1-Ad
		SourceAlphaSaturation, // f, f, f, 1; f = min(As, 1-Ad)
		Factor, // Blend factor
		InverseFactor, // 1-Blend factor
	};

	enum class BlendOperator : uint8_t {
		Add,
		Subtract,
		InverseSubtract,
		Min,
		Max,
	};

	enum class FaceCulling : uint8_t {
		Clockwise,
		CounterClockwise,
	};

	enum class BufferWriting : uint8_t {
		Red,
		Green,
		Blue,
		Alpha,
		Z,
		RGB = (Red | Green | Blue),
		All = (Red | Green | Blue | Alpha | Z)
	};

	BufferWriting operator|(BufferWriting lhs, BufferWriting rhs) {
		return static_cast<BufferWriting>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	BufferWriting operator&(BufferWriting lhs, BufferWriting rhs) {
		return static_cast<BufferWriting>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	struct RenderState {
		PrimitiveType primitiveType{ PrimitiveType::Triangles };
		BlendOperator blendOperator{ BlendOperator::Subtract };
		BlendOperand blendLhs{ BlendOperand::One };
		BlendOperand blendRhs{ BlendOperand::SourceAlpha };
		uint32_t blendFactor{ 0 };
		BufferWriting bufferWriting{ BufferWriting::All };
		std::optional<DepthTest> depthTest {};
		bool msaa{ false };

	};

} // namespace Ortha::Gfx
