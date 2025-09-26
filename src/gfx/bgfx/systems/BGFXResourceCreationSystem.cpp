module;

#include <cstring>
#include <utility>

#include <bgfx/bgfx.h>
#include <entt/entt.hpp>

module Gfx.BGFX.BGFXResourceCreationSystem;

import Core.FileLoadRequest;
import Core.Log;
import Core.ProcessError;
import Core.ResourceHandle;
import Core.ResourceHandleUtils;
import Core.TypeId;
import Gfx.ShaderDescriptor;
import Gfx.ShaderProgramDescriptor;
import Gfx.BGFX.BGFXFrameBuffer;
import Gfx.BGFX.BGFXShader;
import Gfx.BGFX.BGFXShaderProgram;
import Gfx.BGFX.BGFXTexture;
import Gfx.BGFX.BGFXTransientIndexBuffer;
import Gfx.BGFX.BGFXTransientVertexBuffer;
import Gfx.BGFX.BGFXUniform;
import Gfx.BGFX.BGFXVertexLayout;

namespace Gfx::BGFX::BGFXResourceCreationSystemInternal {
	void destroyTexture(entt::registry& registry, const entt::entity entity) {
		if (!registry.all_of<BGFXTexture>(entity)) {
			return;
		}

		auto& texture{ registry.get<BGFXTexture>(entity) };
		if (!bgfx::isValid(texture.handle)) {
			return;
		}

		bgfx::destroy(texture.handle);
		texture.handle = BGFX_INVALID_HANDLE;
	}

	void destroyShader(entt::registry& registry, const entt::entity entity) {
		if (!registry.all_of<BGFXShader>(entity)) {
			return;
		}

		auto& shader{ registry.get<BGFXShader>(entity) };
		if (!bgfx::isValid(shader.handle)) {
			return;
		}

		bgfx::destroy(shader.handle);
		shader.handle = BGFX_INVALID_HANDLE;
	}

	void destroyShaderProgram(entt::registry& registry, const entt::entity entity) {
		if (!registry.all_of<BGFXShaderProgram>(entity)) {
			return;
		}

		auto& program{ registry.get<BGFXShaderProgram>(entity) };
		if (!bgfx::isValid(program.programHandle)) {
			return;
		}

		bgfx::destroy(program.programHandle);
		program.programHandle = BGFX_INVALID_HANDLE;
	}
}

namespace Gfx::BGFX {

	BGFXResourceCreationSystem::BGFXResourceCreationSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	BGFXResourceCreationSystem::~BGFXResourceCreationSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void BGFXResourceCreationSystem::tickSystem(entt::registry& registry) {
		registry.view<const ShaderProgram>(entt::exclude<BGFXUniforms, Core::ProcessError>)
			.each([&registry](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateUniforms(registry, entity, shaderProgram);
			});

		registry
			.view<const ShaderDescriptor, const Core::RawDataResource>(entt::exclude<Core::FileLoadRequest, Core::ProcessError, BGFXShader>)
			.each([&registry](entt::entity entity, const Core::RawDataResource& rawDataResource) {
				_tryCreateShader(registry, entity, rawDataResource);
			});

		registry.view<const ShaderProgram>(entt::exclude<BGFXShaderProgram, Core::ProcessError>)
			.each([&registry](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateShaderProgram(registry, entity, shaderProgram);
			});

		registry.view<const ShaderProgram>(entt::exclude<BGFXVertexLayout, Core::ProcessError>)
			.each([&registry](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateVertexLayout(registry, entity, shaderProgram);
			});

		registry.view<const Image>(entt::exclude<BGFXTexture, Core::ProcessError>)
			.each([&registry](entt::entity entity, const Image& image) {
				_tryCreateTexture(registry, entity, image);
			});

		registry.view<const RenderTexture>(entt::exclude<BGFXFrameBuffer, Core::ProcessError>)
			.each([&registry](const entt::entity entity, const RenderTexture& renderTexture) {
				_tryCreateFrameBuffer(registry, entity, renderTexture);
			});

		registry.view<VertexBuffer>()
			.each([&registry](entt::entity entity, const VertexBuffer& vertexBuffer) {
				if (vertexBuffer.type == VertexBufferType::Transient) {
					_tryCreateTransientVertexBuffer(registry, entity, vertexBuffer);
				}
			});

		registry.view<IndexBuffer>()
			.each([&registry](entt::entity entity, const IndexBuffer& indexBuffer) {
				if (indexBuffer.type == IndexBufferType::Transient) {
					_tryCreateTransientIndexBuffer(registry, entity, indexBuffer);
				}
			});
	}

	void BGFXResourceCreationSystem::_tryCreateShader(entt::registry& registry, entt::entity entity,
		const Core::RawDataResource& rawDataResource) {
		using namespace Core;

		const bgfx::Memory* mem = bgfx::copy(rawDataResource.rawData.data(), rawDataResource.size);
		const bgfx::ShaderHandle shaderHandle = bgfx::createShader(mem);
		if (!bgfx::isValid(shaderHandle)) {
			addProcessError(registry, entity, "Failed to create shader.");
			return;
		}

		registry.emplace<BGFXShader>(entity, shaderHandle);
	}

	void BGFXResourceCreationSystem::_tryCreateShaderProgram(entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {
		using namespace Core;

		const auto& [ vertexShaderEntity, vertexShader] = getResourceAndEntity<BGFXShader>(registry, shaderProgram.vertexShader);
		const auto& [ fragmentShaderEntity, fragmentShader] = getResourceAndEntity<BGFXShader>(registry, shaderProgram.fragmentShader);

		if (!vertexShader || !fragmentShader) {
			const bool shouldRetry = !hasHaltedProcessError(registry, vertexShaderEntity) &&
									 !hasHaltedProcessError(registry, fragmentShaderEntity);

			addProcessError(registry, entity, shouldRetry);

			return;
		}

		if (!bgfx::isValid(vertexShader->handle) ||
			!bgfx::isValid(fragmentShader->handle)) {
			addProcessError(registry, entity, "Invalid shader handles passed to program creation.");
			return;
		}

		bgfx::ProgramHandle program = bgfx::createProgram(vertexShader->handle, fragmentShader->handle);
		if (!bgfx::isValid(program)) {
			addProcessError(registry, entity, "Failed to create shader program handle.");
			return;
		}

		registry.emplace<BGFXShaderProgram>(entity, program);
	}

	void BGFXResourceCreationSystem::_tryCreateVertexLayout(entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {
		using namespace Core;

		//using namespace BGFXSystemInternal;
		bgfx::VertexLayout vertexLayout{};
		vertexLayout.begin();

		for (auto&& inputAttribute : shaderProgram.vertexLayout.attributes) {
			bgfx::Attrib::Enum attribute{};
			switch (inputAttribute.attributeId) {
				case ShaderVertexLayoutAttributeId::Position:
					attribute = bgfx::Attrib::Position;
					break;
				case ShaderVertexLayoutAttributeId::TexCoord0:
					attribute = bgfx::Attrib::TexCoord0;
					break;
				case ShaderVertexLayoutAttributeId::Colour0:
					attribute = bgfx::Attrib::Color0;
					break;
				default:
					addProcessError(registry, entity, "Unhandled vertex attribute type.");
					vertexLayout.end();
					return;
			}

			bgfx::AttribType::Enum attributeType{};
			if (inputAttribute.typeId == Core::TypeId::get<float>()) {
				attributeType = bgfx::AttribType::Float;
			} else if (inputAttribute.typeId == Core::TypeId::get<uint8_t>()) {
				attributeType = bgfx::AttribType::Uint8;
			} else {
				addProcessError(registry, entity, "Unhandled attribute type.");
				vertexLayout.end();
				return;
			}

			vertexLayout.add(attribute, inputAttribute.count, attributeType, inputAttribute.isNormalized);
		}

		vertexLayout.end();

		registry.emplace<BGFXVertexLayout>(entity, vertexLayout);
	}

	void BGFXResourceCreationSystem::_tryCreateUniforms(entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {
		using namespace Core;

		BGFXUniforms bgfxUniforms{};
		for (auto&& uniform : shaderProgram.uniforms) {
			if (bgfxUniforms.uniforms.contains(uniform.name)) {
				addProcessError(registry, entity, "Uniform with duplicate name '{}' encountered.", uniform.name);
				return;
			}

			bgfx::UniformType::Enum uniformType{};
			switch (uniform.type) {
				case ShaderUniformType::Sampler:
					uniformType = bgfx::UniformType::Sampler;
					break;
				case ShaderUniformType::Float4:
					uniformType = bgfx::UniformType::Vec4;
					break;
				default:
					addProcessError(registry, entity, "Unhandled type for uniform '{}'.", uniform.name);
					return;
			}

			bgfx::UniformHandle uniformHandle{ bgfx::createUniform(uniform.name.c_str(), uniformType) };
			if (!bgfx::isValid(uniformHandle)) {
				addProcessError(registry, entity, "Failed to create BGFX uniform '{}'.", uniform.name);
				return;
			}

			bgfxUniforms.uniforms.emplace(uniform.name, uniformHandle);
		}

		registry.emplace<BGFXUniforms>(entity, std::move(bgfxUniforms));
	}

	void BGFXResourceCreationSystem::_tryCreateTexture(entt::registry& registry, entt::entity entity, const Image& imageResource) {
		using namespace Core;

		const bgfx::Memory* mem = bgfx::copy(imageResource.image.data(), imageResource.image.size());
		bgfx::TextureHandle texture = bgfx::createTexture2D(
			imageResource.width, imageResource.height, imageResource.numMips > 1, imageResource.numLayers,
			static_cast<bgfx::TextureFormat::Enum>(imageResource.format), BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, mem);

		if (!bgfx::isValid(texture)) {
			addProcessError(registry, entity, "Failed to create BGFX texture.");
			return;
		}

		registry.emplace<BGFXTexture>(entity, texture);
	}

	void BGFXResourceCreationSystem::_tryCreateFrameBuffer(entt::registry& registry, const entt::entity entity, const RenderTexture& renderTexture) {
		using namespace Core;

		const bgfx::FrameBufferHandle frameBufferHandle = bgfx::createFrameBuffer(renderTexture.width, renderTexture.height, bgfx::TextureFormat::RGBA32F);
		if (!bgfx::isValid(frameBufferHandle)) {
			addProcessError(registry, entity, "Failed to create BGFX frame buffer.");
			return;
		}

		registry.emplace<BGFXFrameBuffer>(entity, frameBufferHandle);
	}

	void BGFXResourceCreationSystem::_tryCreateTransientVertexBuffer(entt::registry& registry, entt::entity& entity, const VertexBuffer& vertexBuffer) {
		using namespace Core;

		if (!registry.all_of<BGFXVertexLayout>(vertexBuffer.vertexLayout)) {
			return;
		}

		const auto& vertexLayout{ registry.get<BGFXVertexLayout>(vertexBuffer.vertexLayout) };
		const uint32_t vertexCount = vertexBuffer.data.size() / vertexLayout.layout.getStride();
		const uint32_t availableVertexCount = bgfx::getAvailTransientVertexBuffer(vertexCount, vertexLayout.layout);

		if (availableVertexCount < vertexCount) {
			Core::logEntry(
				registry, "Requested transient vertex buffer size {} exceeds available amount of {}", vertexCount,
				availableVertexCount);
			return;
		}

		bgfx::TransientVertexBuffer transientVertexBuffer{};
		bgfx::allocTransientVertexBuffer(&transientVertexBuffer, vertexCount, vertexLayout.layout);

		std::memcpy(transientVertexBuffer.data, vertexBuffer.data.data(), vertexBuffer.data.size());
		registry.emplace<BGFXTransientVertexBuffer>(entity, transientVertexBuffer);
	}

	void BGFXResourceCreationSystem::_tryCreateTransientIndexBuffer(entt::registry& registry, entt::entity& entity, const IndexBuffer& indexBuffer) {
		const uint32_t indexCount = indexBuffer.data.size() / (indexBuffer.is32Bit ? 4 : 2);
		const uint32_t availableIndexCount = bgfx::getAvailTransientIndexBuffer(indexCount, indexBuffer.is32Bit);

		if (availableIndexCount < indexCount) {
			Core::logEntry(
				registry, "Requested transient index buffer size {} exceeds available amount of {}", indexCount,
				availableIndexCount);
			return;
		}

		bgfx::TransientIndexBuffer transientIndexBuffer{};
		bgfx::allocTransientIndexBuffer(&transientIndexBuffer, indexCount, indexBuffer.is32Bit);

		std::memcpy(transientIndexBuffer.data, indexBuffer.data.data(), indexBuffer.data.size());

		registry.emplace<BGFXTransientIndexBuffer>(entity, transientIndexBuffer);
	}

	void BGFXResourceCreationSystem::connectEnTTSignals(entt::registry& registry) {
		using namespace BGFXResourceCreationSystemInternal;

		registry.on_destroy<Image>()
			.connect<&destroyTexture>();

		registry.on_destroy<ShaderDescriptor>()
			.connect<&destroyShader>();

		registry.on_destroy<ShaderProgram>()
			.connect<&destroyShaderProgram>();
	}


} // namespace Gfx::BGFX
