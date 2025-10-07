module;

#include <unordered_map>

#include <glm/vec2.hpp>

export module Ortha.Gfx.Font;

export namespace Ortha::Gfx {

	struct FontGlyph {
		glm::ivec2 offset{};
		glm::ivec2 dimensions{};
		glm::vec2 bottomLeftTexCoord{};
		glm::vec2 topRightTexCoord{};
		wchar_t codePoint{};
		int horizontalAdvanceWidth{};
		int leftSideBearing{};
	};

	struct Font {
		int verticalAscent{};
		int verticalDescent{};
		int verticalLineGap{};
		float pixelScale{};

		std::unordered_map<wchar_t, FontGlyph> glyphMap;
		std::unordered_map<wchar_t, std::unordered_map<wchar_t, int>> glyphKerningMap;
	};

} // namespace Ortha::Gfx
