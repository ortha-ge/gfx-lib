module;

#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>

module Ortha.Gfx.BGFX.BGFXRenderSystem;

import Ortha.Core.Log;
import Ortha.Core.Spatial;
import Ortha.RTTI.TypeId;
import Ortha.Core.Window;
import Ortha.Gfx.Camera;
import Ortha.Gfx.IndexBuffer;
import Ortha.Gfx.RenderCommand;
import Ortha.Gfx.RenderState;
import Ortha.Gfx.RenderTexture;
import Ortha.Gfx.ShaderProgram;
import Ortha.Gfx.VertexBuffer;
import Ortha.Gfx.Viewport;
import Ortha.Gfx.ViewportUtilities;
import Ortha.Gfx.BGFX.BGFXContext;
import Ortha.Gfx.BGFX.BGFXFrameBuffer;
import Ortha.Gfx.BGFX.BGFXShader;
import Ortha.Gfx.BGFX.BGFXShaderProgram;
import Ortha.Gfx.BGFX.BGFXState;
import Ortha.Gfx.BGFX.BGFXTexture;
import Ortha.Gfx.BGFX.BGFXTransientIndexBuffer;
import Ortha.Gfx.BGFX.BGFXTransientVertexBuffer;
import Ortha.Gfx.BGFX.BGFXUniform;
import Ortha.Gfx.BGFX.BGFXVertexLayout;
import entt;
import glm;

namespace Ortha::Gfx::BGFX {

	BGFXRenderSystem::BGFXRenderSystem(entt::registry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	BGFXRenderSystem::~BGFXRenderSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void BGFXRenderSystem::tickSystem(entt::registry& registry) {
		using namespace Ortha::Core;

		bgfx::ViewId viewId{ 0 };

		// Clear all windows
		registry.view<Window, BGFXFrameBuffer>()
			.each([this, &viewId](const Window& window, const BGFXFrameBuffer& frameBuffer) {
				bgfx::setViewFrameBuffer(viewId, frameBuffer.handle);
				bgfx::setViewRect(viewId, 0.0f, 0.0f, window.width, window.height);
				bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00000000, 1.0f, 0);
				bgfx::touch(viewId);
				++viewId;
			});

		// Clear all framebuffers
		registry.view<RenderTexture, BGFXFrameBuffer>()
			.each([&viewId](const RenderTexture& renderTexture, const BGFXFrameBuffer& frameBuffer) {
				if (!bgfx::isValid(frameBuffer.handle)) {
					return;
				}

				bgfx::setViewFrameBuffer(viewId, frameBuffer.handle);
				bgfx::setViewRect(viewId, 0.0f, 0.0f, renderTexture.width, renderTexture.height);
				bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00000000, 1.0f, 0);
				bgfx::touch(viewId);
				++viewId;
			});

		struct RenderPassBuckets {
			std::unordered_map<entt::entity, std::vector<RenderCommand>> viewportBuckets;
		};

		// Each render pass and viewport combo is sorted and gets its own viewId.
		std::map<uint16_t, RenderPassBuckets> viewportCommandBuckets;
		registry.view<RenderCommand>().each([&viewportCommandBuckets](const RenderCommand& renderCommand) {
			viewportCommandBuckets[renderCommand.renderPass].viewportBuckets[renderCommand.viewportEntity].emplace_back(renderCommand);
		});

		for (auto&& [_renderPass, viewportCommandBucket] : viewportCommandBuckets) {
			for (auto&& [_viewport, renderCommands] : viewportCommandBucket.viewportBuckets) {
				for (auto&& renderCommand : renderCommands) {
					processRenderCommand(registry, renderCommand, viewId);
				}
				++viewId;
			}
		}

		registry.view<RenderCommand>()
			.each([&registry](entt::entity entity, const RenderCommand&) {
				registry.destroy(entity);
			});

		registry.view<VertexBuffer>()
			.each([&registry](entt::entity entity, const VertexBuffer& vertexBuffer) {
				if (vertexBuffer.type == VertexBufferType::Transient) {
					registry.destroy(entity);
				}
			});

		registry.view<IndexBuffer>()
			.each([&registry](entt::entity entity, const IndexBuffer& indexBuffer) {
				if (indexBuffer.type == IndexBufferType::Transient) {
					registry.destroy(entity);
				}
			});

		bgfx::frame();
	}

	bool BGFXRenderSystem::_tryBindVertexBuffer(
		entt::registry& registry, uint8_t stream, entt::entity vertexBufferEntity, uint32_t offset, uint32_t count) {

		// Vertex buffer is required
		if (vertexBufferEntity == entt::null) {
			return false;
		}

		if (!registry.all_of<VertexBuffer>(vertexBufferEntity)) {
			// Process error??
			return false;
		}

		const auto& vertexBuffer{ registry.get<VertexBuffer>(vertexBufferEntity) };
		switch (vertexBuffer.type) {
			case VertexBufferType::Transient: {
				if (!registry.all_of<BGFXTransientVertexBuffer>(vertexBufferEntity)) {
					return false;
				}

				const auto& bgfxTransientVertexBuffer{ registry.get<BGFXTransientVertexBuffer>(vertexBufferEntity) };
				bgfx::setVertexBuffer(stream, &bgfxTransientVertexBuffer.buffer, offset, count);
				break;
			}
		}

		return true;
	}
	bool BGFXRenderSystem::_tryBindIndexBuffer(
		entt::registry& registry, entt::entity indexBufferEntity, uint32_t offset, uint32_t count) {

		// Index buffer is optional
		if (indexBufferEntity == entt::null) {
			return true;
		}

		// If we are specifying an entity it should have a valid index buffer
		if (!registry.all_of<IndexBuffer>(indexBufferEntity)) {
			return false;
		}

		const auto& indexBuffer{ registry.get<IndexBuffer>(indexBufferEntity) };
		switch (indexBuffer.type) {
			case IndexBufferType::Transient: {
				if (!registry.all_of<BGFXTransientIndexBuffer>(indexBufferEntity)) {
					return false;
				}

				const auto& bgfxTransientIndexBuffer{ registry.get<BGFXTransientIndexBuffer>(indexBufferEntity) };
				bgfx::setIndexBuffer(&bgfxTransientIndexBuffer.buffer, offset, count);
				break;
			}
		}

		return true;
	}

	bool BGFXRenderSystem::_tryBindFrameBuffer(entt::registry& registry, uint8_t viewId, entt::entity renderTarget) {
		// Default render target is null
		if (renderTarget == entt::null) {
			bgfx::setViewFrameBuffer(viewId, BGFX_INVALID_HANDLE);
			return true;
		}

		if (!registry.all_of<BGFXFrameBuffer>(renderTarget)) {
			return false;
		}

		const auto& frameBuffer{ registry.get<BGFXFrameBuffer>(renderTarget) };
		if (!bgfx::isValid(frameBuffer.handle)) {
			return false;
		}

		bgfx::setViewFrameBuffer(viewId, frameBuffer.handle);
		return true;
	}

	bool BGFXRenderSystem::_tryBindUniforms(
		entt::registry& registry, const BGFXUniforms& bgfxUniforms,
		const std::unordered_map<std::string, Ortha::RTTI::Any>& uniformData) {

		for (auto&& [uniformName, uniformValueAny] : uniformData) {
			if (!bgfxUniforms.uniforms.contains(uniformName)) {
				Core::logEntry(registry, "Render command missing uniform '{}'", uniformName);
				return false;
			}

			bgfx::UniformHandle uniformHandle = bgfxUniforms.uniforms.at(uniformName);
			if (!bgfx::isValid(uniformHandle)) {
				return false;
			}

			const auto& uniformTypeId{ uniformValueAny.getTypeId() };
			if (uniformTypeId == Ortha::RTTI::TypeId::get<entt::entity>()) {
				const auto uniformDataEntity = *static_cast<const entt::entity*>(uniformValueAny.getTypeInstance().getInstance());
				if (!_tryBindEntityUniform(registry, uniformHandle, uniformDataEntity)) {
					return false;
				}
			} else if (uniformTypeId == Ortha::RTTI::TypeId::get<glm::vec4>()) {
				const auto* uniformColour = static_cast<const glm::vec4*>(uniformValueAny.getTypeInstance().getInstance());
				bgfx::setUniform(uniformHandle, uniformColour);
			} else {
				// TODO: Log Unhandled
				return false;
			}
		}

		return true;
	}

	bool BGFXRenderSystem::_tryBindEntityUniform(entt::registry& registry, const bgfx::UniformHandle& uniform, entt::entity entity) {
		// Textures
		if (registry.all_of<BGFXTexture>(entity)) {
			const BGFXTexture& bgfxTexture{ registry.get<BGFXTexture>(entity) };
			return _tryBindTextureUniform(registry, uniform, bgfxTexture);
		}

		// FrameBuffers
		if (registry.all_of<BGFXFrameBuffer>(entity)) {
			const BGFXFrameBuffer& bgfxFrameBuffer{ registry.get<BGFXFrameBuffer>(entity) };
			return _tryBindFrameBufferUniform(registry, uniform, bgfxFrameBuffer);
		}

		// TODO: Log Unhandled
		return false;
	}

	bool BGFXRenderSystem::_tryBindTextureUniform(entt::registry& registry, const bgfx::UniformHandle& uniform, const BGFXTexture& bgfxTexture) {
		if (!bgfx::isValid(bgfxTexture.handle)) {
			return false;
		}

		bgfx::setTexture(0, uniform, bgfxTexture.handle);
		return true;
	}

	bool BGFXRenderSystem::_tryBindFrameBufferUniform(entt::registry& registry, const bgfx::UniformHandle& uniform, const BGFXFrameBuffer& bgfxFrameBuffer) {
		if (!bgfx::isValid(bgfxFrameBuffer.handle)) {
			return false;
		}

		const bgfx::TextureHandle bgfxFrameBufferTexture{ bgfx::getTexture(bgfxFrameBuffer.handle) };
		if (!bgfx::isValid(bgfxFrameBufferTexture)) {
			return false;
		}

		bgfx::setTexture(0, uniform, bgfxFrameBufferTexture);
		return true;
	}

	void BGFXRenderSystem::processRenderCommand(entt::registry& registry, const RenderCommand& renderCommand, bgfx::ViewId viewId) {
		using namespace Ortha::Core;

		// Wait for the shader program to be initialized
		if (!registry.all_of<BGFXShaderProgram, BGFXUniforms>(renderCommand.shaderProgram)) {
			return;
		}

		// Validate the viewport and camera
		if (renderCommand.viewportEntity == entt::null) {
			return;
		}

		if (!registry.all_of<Viewport>(renderCommand.viewportEntity)) {
			return;
		}

		const Viewport& viewport{ registry.get<Viewport>(renderCommand.viewportEntity) };

		// Get the render target dimensions
		const auto viewportScreenRect = getViewportScreenRect(registry, viewport);
		if (!viewportScreenRect) {
			return;
		}

		glm::ivec2 viewportScreenSize = getViewportScreenSize(*viewportScreenRect);

		bgfx::setViewRect(viewId,
			viewportScreenRect->bottomLeft.x,
			viewportScreenRect->topRight.y,
			viewportScreenSize.x,
			viewportScreenSize.y);

		// Set the scissor rect for the submit call.
		if (renderCommand.scissorRect) {
			const glm::ivec4& scissorRect{ *renderCommand.scissorRect };
			bgfx::setScissor(scissorRect.x, scissorRect.y, scissorRect.z, scissorRect.w);
		}

		// Set up the model matrix
		bgfx::setTransform(&renderCommand.modelMatrix);

		// Set up the view-projection matrix
		// TODO: Camera settings like ortho/projection
		glm::mat4 projectionMatrix;
		bx::mtxOrtho(&projectionMatrix[0][0], viewportScreenRect->bottomLeft.x, viewportScreenRect->topRight.x,
			viewportScreenRect->bottomLeft.y, viewportScreenRect->topRight.y,
			0.0f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);

		bgfx::setViewTransform(viewId, &renderCommand.viewMatrix, &projectionMatrix);

		// Set the render state
		bgfx::setState(getBGFXRenderState(renderCommand.renderState));

		// Bind the resources
		if (!_tryBindVertexBuffer(registry, 0, renderCommand.vertexBuffer, renderCommand.vertexOffset, renderCommand.vertexCount)) {
			return;
		}

		if (!_tryBindIndexBuffer(registry, renderCommand.indexBuffer, renderCommand.indexOffset, renderCommand.indexCount)) {
			return;
		}

		if (!_tryBindFrameBuffer(registry, viewId, viewport.renderTarget)) {
			return;
		}

		// Bind the uniform data
		const auto& bgfxUniforms{ registry.get<BGFXUniforms>(renderCommand.shaderProgram) };
		if (!_tryBindUniforms(registry, bgfxUniforms, renderCommand.uniformData)) {
			return;
		}

		// Submit the draw call
		const auto& bgfxShaderProgram{ registry.get<BGFXShaderProgram>(renderCommand.shaderProgram) };
		bgfx::submit(viewId, bgfxShaderProgram.programHandle, renderCommand.sortDepth);
	}

} // namespace Ortha::Gfx::BGFX
