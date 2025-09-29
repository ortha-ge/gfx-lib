module;

#include <optional>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION

#include <bimg/decode.h>
#include <stb_truetype.h>

module Gfx.FontLoadSystem;

import Core.FileLoadRequest;
import Core.ProcessError;
import Core.RawDataResource;
import Gfx.Font;
import Gfx.FontDescriptor;
import Gfx.Image;
import entt;
import glm;

namespace Gfx::FontLoadSystemInternal {

	struct StbFont {
		stbtt_fontinfo fontInfo{};
		float pixelScale{};
		int verticalAscent{};
		int verticalDescent{};
		int verticalLineGap{};
	};

	struct StbGlyph {
		int unicodeCodepoint{};
		int glyphIndex{};
		std::vector<uint8_t> bitmapData{};
		glm::ivec2 offset{};
		glm::ivec2 dimensions{};
		int horizontalAdvanceWidth{};
		int leftSideBearing{};
	};

	void tryCreateStbFont(entt::registry& registry, const entt::entity entity, const Core::RawDataResource& rawData) {
		using namespace Core;

		constexpr int fontFaceIndex = 0;
		stbtt_fontinfo info{};
		if (stbtt_InitFont(&info, rawData.rawData.data(), fontFaceIndex) == 0) {
			addProcessError(registry, entity, "Failed to initialize truetype font.");
			return;
		}

		StbFont font;

		int verticalAscent, verticalDescent, verticalLineGap;
		stbtt_GetFontVMetrics(&info, &verticalAscent, &verticalDescent, &verticalLineGap);
		font.verticalAscent = verticalAscent;
		font.verticalDescent = verticalDescent;
		font.verticalLineGap = verticalLineGap;

		font.pixelScale = stbtt_ScaleForMappingEmToPixels(&info, 45.0f);
		font.fontInfo = info;

		registry.emplace<StbFont>(entity, font);
	}

	std::optional<StbGlyph> tryCreateStbGlyphBitmap(
		const stbtt_fontinfo& fontInfo, int unicodeCodepoint, int bpp,
		float pixelScale) {
		int glyphIndex = stbtt_FindGlyphIndex(&fontInfo, unicodeCodepoint);
		if (glyphIndex == 0) {
			return std::nullopt;
		}

		int advanceWidth, leftSideBearing;
		stbtt_GetGlyphHMetrics(&fontInfo, glyphIndex, &advanceWidth, &leftSideBearing);

		glm::ivec2 bottomLeft, topRight;
		stbtt_GetGlyphBitmapBox(
			&fontInfo, glyphIndex, pixelScale, pixelScale, &bottomLeft.x, &bottomLeft.y, &topRight.x, &topRight.y);

		glm::ivec2 dimensions;
		dimensions.x = topRight.x - bottomLeft.x;
		dimensions.y = topRight.y - bottomLeft.y;

		std::vector<uint8_t> bitmapData;
		bitmapData.resize(dimensions.x * dimensions.y * bpp);

		stbtt_MakeGlyphBitmap(
			&fontInfo, bitmapData.data(), dimensions.x, dimensions.y, dimensions.x * bpp, pixelScale, pixelScale, glyphIndex);

		StbGlyph glyph;
		glyph.unicodeCodepoint = unicodeCodepoint;
		glyph.glyphIndex = glyphIndex;
		glyph.bitmapData = std::move(bitmapData);
		glyph.offset = bottomLeft;
		glyph.dimensions = dimensions;
		glyph.horizontalAdvanceWidth = advanceWidth;
		glyph.leftSideBearing = leftSideBearing;
		return glyph;
	}

	void tryCreateStbGlyphBitmaps(
		entt::registry& registry, const entt::entity entity, const StbFont& stbFont,
		const Core::RawDataResource& rawData) {
		using namespace Core;

		constexpr int bpp = 1;
		constexpr int characterCount{ 255 };

		std::vector<StbGlyph> glyphBitmaps;
		glyphBitmaps.reserve(characterCount);
		int32_t totalPixelWidth{ 0zu };
		int32_t maxPixelHeight{ 0zu };

		for (int i = 0; i < characterCount; ++i) {
			auto glyphBitmap = tryCreateStbGlyphBitmap(stbFont.fontInfo, i, bpp, stbFont.pixelScale);
			if (!glyphBitmap) {
				continue;
			}

			totalPixelWidth += glyphBitmap->dimensions.x;
			maxPixelHeight = std::max(maxPixelHeight, glyphBitmap->dimensions.y);
			glyphBitmaps.push_back(std::move(*glyphBitmap));
		}

		std::vector<uint8_t> bitmapBuffer;
		bitmapBuffer.resize(totalPixelWidth * maxPixelHeight * bpp);

		size_t glyphStartIndex = 0;
		for (auto&& glyphBitmap : glyphBitmaps) {
			for (size_t y = 0; y < glyphBitmap.dimensions.y; ++y) {
				const size_t glyphWriteIndex = glyphStartIndex + y * totalPixelWidth;
				assert(glyphWriteIndex < bitmapBuffer.size());
				assert(glyphWriteIndex + glyphBitmap.dimensions.x <= bitmapBuffer.size());

				const size_t glyphReadIndex = y * glyphBitmap.dimensions.x;

				auto* glyphWriteHead = &bitmapBuffer[glyphWriteIndex];
				memcpy(glyphWriteHead, &glyphBitmap.bitmapData[glyphReadIndex], glyphBitmap.dimensions.x);
			}

			// Set for the next glyph to write to
			glyphStartIndex += glyphBitmap.dimensions.x * bpp;
		}

		Image image;
		image.image = std::move(bitmapBuffer);
		image.format = bimg::TextureFormat::R8;
		image.offset = 0;
		image.width = totalPixelWidth;
		image.height = maxPixelHeight;
		image.depth = 1;
		image.numLayers = 1;
		image.numMips = 1;
		image.hasAlpha = true;
		image.cubeMap = false;
		image.ktx = false;
		image.ktxLE = false;
		image.pvr3 = false;
		image.srgb = false;

		registry.emplace<Image>(entity, std::move(image));

		Font font;
		font.verticalAscent = stbFont.verticalAscent;
		font.verticalDescent = stbFont.verticalDescent;
		font.verticalLineGap = stbFont.verticalLineGap;
		font.pixelScale = stbFont.pixelScale;

		glyphStartIndex = 0;
		for (size_t glyphIndex = 0; glyphIndex < glyphBitmaps.size(); ++glyphIndex) {
			const auto& stbGlyph{ glyphBitmaps[glyphIndex] };
			FontGlyph glyph;
			glyph.offset = stbGlyph.offset;
			glyph.dimensions = stbGlyph.dimensions;
			glyph.codePoint = stbGlyph.unicodeCodepoint;
			glyph.horizontalAdvanceWidth = stbGlyph.horizontalAdvanceWidth;
			glyph.leftSideBearing = stbGlyph.leftSideBearing;

			auto& glyphKerningMap{ font.glyphKerningMap[glyph.codePoint] };

			for (size_t otherGlyphIndex = 0; otherGlyphIndex < glyphBitmaps.size(); ++otherGlyphIndex) {
				const auto& otherStbGlyph{ glyphBitmaps[otherGlyphIndex] };
				int kerning = stbtt_GetGlyphKernAdvance(&stbFont.fontInfo, stbGlyph.unicodeCodepoint, otherStbGlyph.unicodeCodepoint);

				glyphKerningMap[otherStbGlyph.unicodeCodepoint] = kerning;
			}

			const float x0 = static_cast<float>(glyphStartIndex) / static_cast<float>(totalPixelWidth);
			const float y0 = static_cast<float>(0.0f) / static_cast<float>(maxPixelHeight);
			const float x1 = static_cast<float>(glyphStartIndex + stbGlyph.dimensions.x) / static_cast<float>(totalPixelWidth);
			const float y1 = static_cast<float>(stbGlyph.dimensions.y) / static_cast<float>(maxPixelHeight);

			glyph.bottomLeftTexCoord = glm::vec2(x0, y0);
			glyph.topRightTexCoord = glm::vec2(x1, y1);
			glyphStartIndex += stbGlyph.dimensions.x * bpp;

			font.glyphMap.emplace(glyph.codePoint, glyph);
		}

		registry.emplace<Font>(entity, std::move(font));
	}

} // namespace Gfx::FontLoadSystemInternal

namespace Gfx {



	FontLoadSystem::FontLoadSystem(entt::registry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	FontLoadSystem::~FontLoadSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void FontLoadSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace FontLoadSystemInternal;

		registry.view<FontDescriptor, RawDataResource>(entt::exclude<FileLoadRequest, ProcessError, StbFont>)
			.each([&registry](const entt::entity entity, const RawDataResource& rawData) {
				tryCreateStbFont(registry, entity, rawData);
			});

		registry.view<StbFont, RawDataResource>(entt::exclude<FileLoadRequest, ProcessError, Image>)
			.each([&registry](const entt::entity entity, const StbFont& stbFont, const RawDataResource& rawData) {
				tryCreateStbGlyphBitmaps(registry, entity, stbFont, rawData);
			});
	}

} // namespace Gfx
