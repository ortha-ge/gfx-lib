module;

#include <utility>

#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

module Gfx.BGFX.BGFXRenderSystem;

import Core.Log;
import Core.Spatial;
import Core.TypeId;
import Gfx.Camera;
import Gfx.IndexBuffer;
import Gfx.RenderCommand;
import Gfx.ShaderProgram;
import Gfx.VertexBuffer;
import Gfx.Viewport;
import Gfx.BGFX.BGFXFrameBuffer;
import Gfx.BGFX.BGFXShader;
import Gfx.BGFX.BGFXShaderProgram;
import Gfx.BGFX.BGFXTexture;
import Gfx.BGFX.BGFXTransientIndexBuffer;
import Gfx.BGFX.BGFXTransientVertexBuffer;
import Gfx.BGFX.BGFXUniform;
import Gfx.BGFX.BGFXVertexLayout;

namespace Gfx::BGFX {

	BGFXRenderSystem::BGFXRenderSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
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

		std::unordered_map<bgfx::ViewId, std::vector<RenderCommand>> commandBuckets;
		registry.view<RenderCommand>().each([&commandBuckets, &registry](const RenderCommand& renderCommand) {
			commandBuckets[renderCommand.renderPass].emplace_back(renderCommand);
		});

		for (auto&& [viewId, renderCommands] : commandBuckets) {
			//std::ranges::sort(renderCommands, [](const auto& lhs, const auto& rhs) { return lhs.sortDepth < rhs.sortDepth; });
			for (auto&& renderCommand : renderCommands) {
				processRenderCommand(registry, renderCommand);
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
		const std::unordered_map<std::string, Core::Any>& uniformData) {

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
			if (uniformTypeId == Core::TypeId::get<entt::entity>()) {
				const auto uniformDataEntity = *static_cast<const entt::entity*>(uniformValueAny.getInstance());
				if (!_tryBindEntityUniform(registry, uniformHandle, uniformDataEntity)) {
					return false;
				}
			} else if (uniformTypeId == Core::TypeId::get<glm::vec4>()) {
				const auto* uniformColour = static_cast<const glm::vec4*>(uniformValueAny.getInstance());
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

	void BGFXRenderSystem::processRenderCommand(entt::registry& registry, const RenderCommand& renderCommand) {
		using namespace Core;

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

		// TODO: viewId should be allocated uniquely per viewport, and is no longer needed for sprite depth sorting.
		bgfx::ViewId viewId = renderCommand.renderPass;

		// HACK: Add to the viewId if there is a render target
		if (viewport.renderTarget != entt::null) {
			// HACK: Don't render the render target test to the render target.
			if (viewId > 200) {
				return;
			}

			viewId += 15;
		}

		// TODO: Should be coming from the render target or window.
		constexpr float width = 1360.0f;
		constexpr float height = 768.0f;

		const float left = viewport.offset.x * width;
		const float right = viewport.dimensions.x * width;
		const float top = viewport.offset.y * height;
		const float bottom = viewport.dimensions.y * height;

		bgfx::setViewRect(viewId, left, top, right - left, bottom - top);

		// Set the scissor rect for the submit call.
		if (renderCommand.scissorRect) {
			const glm::ivec4& scissorRect{ *renderCommand.scissorRect };
			bgfx::setScissor(scissorRect.x, scissorRect.y, scissorRect.z, scissorRect.w);
		}

		if (viewport.camera == entt::null) {
			return;
		}

		if (!registry.all_of<Camera, Spatial>(viewport.camera)) {
			return;
		}

		// Set up the model matrix
		bgfx::setTransform(&renderCommand.modelMatrix);

		// Set up the view-projection matrix
		const auto& [camera, cameraSpatial] = registry.get<Camera, Spatial>(viewport.camera);

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), cameraSpatial.position);
		glm::mat4 rotation = glm::mat4_cast(cameraSpatial.rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraSpatial.scale);
		glm::mat4 viewMatrix{ glm::inverse(translation * rotation * scale) };

		// TODO: Camera settings like ortho/projection
		glm::mat4 projectionMatrix;
		bx::mtxOrtho(&projectionMatrix[0][0], left, right, bottom, top, 0.0f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(viewId, &viewMatrix, &projectionMatrix);

		if (renderCommand.bgfxState) {
			bgfx::setState(*renderCommand.bgfxState);
		} else {
			// Set render states.
			bgfx::setState(
				0 | BGFX_STATE_WRITE_RGB |
				BGFX_STATE_WRITE_A
				//| BGFX_STATE_WRITE_Z
				//| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA) |
				BGFX_STATE_BLEND_EQUATION_ADD
				//| BGFX_STATE_MSAA
			);
		}

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

} // namespace Gfx::BGFX
