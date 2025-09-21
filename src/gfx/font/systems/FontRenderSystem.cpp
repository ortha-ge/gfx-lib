module;

#include <utility>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

module Gfx.FontRenderSystem;

import Core.Any;
import Core.JsonTypeLoaderAdapter;
import Core.ResourceHandleUtils;
import Core.ResourceLoadRequest;
import Core.GlobalSpatial;
import Core.TypeLoader;
import Gfx.Font;
import Gfx.FontObject;
import Gfx.Image;
import Gfx.IndexBuffer;
import Gfx.RenderCommand;
import Gfx.ShaderProgram;
import Gfx.VertexBuffer;
import Gfx.Viewport;
import Gfx.Reflection.ShaderProgramDescriptor;

namespace Gfx::FontRenderSystemInternal {

	struct FontVertex {
		glm::vec3 position;
		glm::vec2 texCoord0;
		glm::vec3 color;
	};

	RenderCommand createFontObjectRenderCommand(
		entt::registry& registry, const FontObject& fontObject, const Font& font, const entt::entity viewportEntity,
		const entt::entity shaderProgramEntity, const entt::entity imageEntity, const Core::GlobalSpatial& spatial) {

		RenderCommand renderCommand;
		renderCommand.viewportEntity = viewportEntity;
		renderCommand.renderPass = 20;

		renderCommand.shaderProgram = shaderProgramEntity;
		renderCommand.uniformData["s_texColour"] = Core::Any(entt::entity{ imageEntity });
		renderCommand.uniformData["u_alphaColour"] = Core::Any(glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f });

		size_t vertexCount = std::ranges::count_if(fontObject.text, [](auto&& character) {
			return character != '\n';
		});

		VertexBuffer vertexBuffer;
		vertexBuffer.data.resize(sizeof(FontVertex) * 4 * vertexCount);
		vertexBuffer.type = VertexBufferType::Transient;

		IndexBuffer indexBuffer;
		indexBuffer.data.resize(sizeof(uint16_t) * 6 * vertexCount);
		indexBuffer.is32Bit = false;

		glm::vec3 glyphWritePosition = spatial.position;
		for (size_t i = 0, quadIndex = 0; i < fontObject.text.size(); ++i) {
			wchar_t codepoint = fontObject.text[i];
			if (codepoint == L'\n') {
				glyphWritePosition.x = spatial.position.x;
				glyphWritePosition.y += static_cast<float>(font.verticalAscent) * font.pixelScale;

				continue;
			}

			// Kerning
			if (i > 0) {
				wchar_t previousCodepoint = fontObject.text[i - 1];
				if (font.glyphKerningMap.contains(previousCodepoint)) {
					const auto& glyphKerningMap{ font.glyphKerningMap.at(previousCodepoint) };
					if (glyphKerningMap.contains(codepoint)) {
						const int kerning = glyphKerningMap.at(codepoint);
						glyphWritePosition.x += static_cast<float>(kerning);
					}
				}
			}

			// Glyph offset
			const auto& glyph{ font.glyphMap.at(codepoint) };
			const glm::vec3 currentGlyphPosition = glyphWritePosition + glm::vec3{ glyph.offset.x, glyph.offset.y, 0.0f };

			// Glyph texture coords
			const auto& bottomLeftTexCoord{ glyph.bottomLeftTexCoord };
			const auto& topRightTexCoord{ glyph.topRightTexCoord };

			const size_t startVertex = quadIndex * 4;
			auto* vertexHead = reinterpret_cast<FontVertex*>(&vertexBuffer.data[sizeof(FontVertex) * startVertex]);
			vertexHead[0] = { currentGlyphPosition + glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ bottomLeftTexCoord.x, bottomLeftTexCoord.y }, glm::vec3{ 1.0f, 1.0f, 1.0f } };
			vertexHead[1] = { currentGlyphPosition + glm::vec3{ 0.0f, glyph.dimensions.y, 0.0f }, glm::vec2{ bottomLeftTexCoord.x, topRightTexCoord.y }, glm::vec3{ 1.0f, 1.0f, 1.0f } };
			vertexHead[2] = { currentGlyphPosition + glm::vec3{ glyph.dimensions.x, glyph.dimensions.y, 0.0f }, glm::vec2{ topRightTexCoord.x, topRightTexCoord.y }, glm::vec3{ 1.0f, 1.0f, 1.0f } };
			vertexHead[3] = { currentGlyphPosition + glm::vec3{ glyph.dimensions.x, 0.0f, 0.0f }, glm::vec2{ topRightTexCoord.x, bottomLeftTexCoord.y }, glm::vec3{ 1.0f, 1.0f, 1.0f } };

			const size_t startIndex = quadIndex * 6;
			auto* indexHead = reinterpret_cast<uint16_t*>(&indexBuffer.data[sizeof(uint16_t) * startIndex]);
			indexHead[0] = startVertex;
			indexHead[1] = startVertex + 1;
			indexHead[2] = startVertex + 3;

			indexHead[3] = startVertex + 3;
			indexHead[4] = startVertex + 1;
			indexHead[5] = startVertex + 2;

			glyphWritePosition.x += static_cast<float>(glyph.horizontalAdvanceWidth) * font.pixelScale;
			++quadIndex;
		}

		const entt::entity vertexBufferEntity = registry.create();
		registry.emplace<VertexBuffer>(vertexBufferEntity, std::move(vertexBuffer));
		renderCommand.vertexBuffer = vertexBufferEntity;
		renderCommand.vertexCount = 4 * fontObject.text.size();

		const entt::entity indexBufferEntity = registry.create();
		registry.emplace<IndexBuffer>(indexBufferEntity, std::move(indexBuffer));
		renderCommand.indexBuffer = indexBufferEntity;
		renderCommand.indexCount = 6 * fontObject.text.size();

		return renderCommand;
	}

} // namespace Gfx::FontRenderSystemInternal

namespace Gfx {

	FontRenderSystem::FontRenderSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		auto typeLoader = std::make_shared<Core::JsonTypeLoaderAdapter<ShaderProgramDescriptor>>();
		mFontShaderProgram = Core::ResourceLoadRequest::create<Core::TypeLoader>(registry, "assets/shaders/font.json", typeLoader);

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	FontRenderSystem::~FontRenderSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void FontRenderSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace FontRenderSystemInternal;

		auto viewportView = registry.view<Viewport>();
		if (viewportView.empty()) {
			return;
		}

		const entt::entity viewportEntity = viewportView.front();

		registry.view<FontObject, GlobalSpatial>()
			.each([this, &registry, viewportEntity](const FontObject& fontObject, const GlobalSpatial& spatial) {

				auto&& [imageEntity, image] = getResourceAndEntity<Image>(registry, fontObject.font);
				if (!image) {
					return;
				}

				const auto* font = getResource<Font>(registry, fontObject.font);
				if (!font) {
					return;
				}

				// Validate text
				for (size_t i = 0; i < fontObject.text.size(); ++i) {
					wchar_t codepoint = fontObject.text[i];
					if (codepoint == '\n') {
						continue;
					}

					if (!font->glyphMap.contains(codepoint)) {
						return;
					}

					if (i > 0) {
						wchar_t previousCodepoint = fontObject.text[i - 1];
						if (font->glyphKerningMap.contains(previousCodepoint)) {
							if (!font->glyphKerningMap.at(previousCodepoint).contains(codepoint)) {
								return;
							}
						}
					}
				}

				auto&& [shaderProgramEntity, shaderProgram] = getResourceAndEntity<ShaderProgram>(registry, mFontShaderProgram);
				if (!shaderProgram) {
					return;
				}

				RenderCommand renderCommand = createFontObjectRenderCommand(registry, fontObject, *font, viewportEntity, shaderProgramEntity, imageEntity, spatial);

				const entt::entity renderCommandEntity = registry.create();
				registry.emplace<RenderCommand>(renderCommandEntity, renderCommand);
			});
	}

} // namespace Gfx
