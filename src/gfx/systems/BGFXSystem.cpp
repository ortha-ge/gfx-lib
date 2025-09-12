module;

#include <cstring>

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

module Gfx.BGFXSystem;

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.Log;
import Core.NativeWindowHandles;
import Core.ProcessError;
import Core.RawDataResource;
import Core.ResourceHandle;
import Core.ResourceHandleUtils;
import Core.Spatial;
import Core.TypeId;
import Core.Window;
import Gfx.BGFXContext;
import Gfx.BGFXDrawCallback;
import Gfx.Camera;
import Gfx.Image;
import Gfx.IndexBuffer;
import Gfx.Material;
import Gfx.RenderCommand;
import Gfx.RenderObject;
import Gfx.ShaderDescriptor;
import Gfx.ShaderPlatformInfo;
import Gfx.ShaderProgram;
import Gfx.ShaderProgramDescriptor;
import Gfx.TextureCoordinates;
import Gfx.VertexBuffer;
import Gfx.Viewport;

namespace Gfx::BGFXSystemInternal {

	struct BGFXTexture {
		bgfx::TextureHandle textureHandle{};
	};

	struct BGFXShader {
		bgfx::ShaderHandle shaderHandle{};
	};

	struct BGFXProgram {
		bgfx::ProgramHandle programHandle{};
	};

	struct BGFXVertexLayout {
		bgfx::VertexLayout vertexLayout{};
	};

	struct BGFXUniforms {
		std::unordered_map<std::string, bgfx::UniformHandle> uniforms{};
	};

	struct BGFXCallbacks : bgfx::CallbackI {

		BGFXCallbacks(entt::registry& registry, entt::entity entity)
			: registry(registry)
			, entity(entity) {
		}

		void trace(const char* _filePath, uint16_t _line, const char* _format, ...) {
			va_list argList;
			va_start(argList, _format);

			traceVargs(_filePath, _line, _format, argList);

			va_end(argList);
		}

		void fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) override {
			trace(_filePath, _line, "BGFX FATAL 0x%08x: %s\n", _code, _str);

			if (bgfx::Fatal::DebugCheck == _code) {
				bx::debugBreak();
			}
			else {
				abort();
			}
		}

		void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) override {
			char temp[2048];
			char* out = temp;
			va_list argListCopy;
			va_copy(argListCopy, _argList);
			int32_t len = snprintf(out, sizeof(temp), "%s (%d): ", _filePath, _line);
			int32_t total = len + vsnprintf(out + len, sizeof(temp)-len, _format, argListCopy);
			va_end(argListCopy);
			if ( (int32_t)sizeof(temp) < total) {
				out = (char*)BX_STACK_ALLOC(total+1);
				bx::memCopy(out, temp, len);
				vsnprintf(out + len, total-len, _format, _argList);
			}
			out[total - 1] = '\0';

			Core::logEntry(registry, entity, out);
		}

		void profilerBegin(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) override {

		}

		void profilerBeginLiteral(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) override {

		}

		void profilerEnd() override {

		}

		uint32_t cacheReadSize(uint64_t _id) override {
			return 0u;
		}

		bool cacheRead(uint64_t _id, void* _data, uint32_t _size) override {
			return false;
		}

		void cacheWrite(uint64_t _id, const void* _data, uint32_t _size) override {

		}

		void screenShot(
			const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data,
			uint32_t _size, bool _yflip) override {

		}

		void captureBegin(
			uint32_t _width, uint32_t _height, uint32_t _pitch, bgfx::TextureFormat::Enum _format,
			bool _yflip) override {

		}

		void captureEnd() override {

		}

		void captureFrame(const void* _data, uint32_t _size) override {

		}

	private:

		entt::registry& registry;
		entt::entity entity;

	};

	void processRenderCommand(entt::registry& registry, const RenderCommand& renderCommand) {
		if (!registry.all_of<Viewport>(renderCommand.viewportEntity)) {
			return;
		}

		const Viewport& viewport{ registry.get<Viewport>(renderCommand.viewportEntity) };
		if (!registry.all_of<Camera, Core::Spatial>(viewport.camera)) {
			return;
		}

		// const auto& camera{ registry.get<Camera>(viewport.camera) };
		const auto& spatial{ registry.get<Core::Spatial>(viewport.camera) };
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), spatial.position);
		glm::mat4 rotation = glm::mat4_cast(spatial.rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), spatial.scale);

		glm::mat4 viewMatrix = glm::inverse(translation * rotation * scale);

		constexpr float width = 1360.0f;
		constexpr float height = 768.0f;

		const float left = viewport.offset.x * width;
		const float right = viewport.dimensions.x * width;
		const float top = viewport.offset.y * height;
		const float bottom = viewport.dimensions.y * height;

		bgfx::setViewRect(renderCommand.renderPass, left, top, right - left, bottom - top);

		float proj[16];
		bx::mtxOrtho(proj, left, right, bottom, top, 0.0f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(renderCommand.renderPass, &viewMatrix, proj);

		if (!registry.all_of<VertexBuffer>(renderCommand.vertexBuffer) ||
			!registry.all_of<IndexBuffer>(renderCommand.indexBuffer) ||
			!registry.all_of<ShaderProgram>(renderCommand.shaderProgram)) {

			return;
		}

		auto& vertexBuffer{ registry.get<VertexBuffer>(renderCommand.vertexBuffer) };
		auto& indexBuffer{ registry.get<IndexBuffer>(renderCommand.indexBuffer) };

		if (vertexBuffer.type != VertexBufferType::Transient ||
			indexBuffer.type != IndexBufferType::Transient) {
			return;
		}

		if (!registry.all_of<BGFXProgram, BGFXUniforms, BGFXVertexLayout>(renderCommand.shaderProgram)) {
			return;
		}

		const auto& vertexLayout{ registry.get<BGFXVertexLayout>(renderCommand.shaderProgram) };

		constexpr uint32_t maxVertices = (32 << 10);
		constexpr uint32_t maxIndices = (32 << 10);

		if (bgfx::getAvailTransientVertexBuffer(maxVertices, vertexLayout.vertexLayout) < maxVertices ||
			bgfx::getAvailTransientIndexBuffer(maxIndices) < maxIndices) {
			return;
		}

		bgfx::TransientVertexBuffer transientVertexBuffer{};
		bgfx::allocTransientVertexBuffer(&transientVertexBuffer, maxVertices, vertexLayout.vertexLayout);

		std::memcpy(transientVertexBuffer.data, vertexBuffer.data.data(), vertexBuffer.data.size());

		bgfx::TransientIndexBuffer transientIndexBuffer{};
		bgfx::allocTransientIndexBuffer(&transientIndexBuffer, maxIndices, indexBuffer.is32Bit);

		std::memcpy(transientIndexBuffer.data, indexBuffer.data.data(), indexBuffer.data.size());

		const auto& bgfxProgram = registry.get<BGFXProgram>(renderCommand.shaderProgram);
		const auto& bgfxUniforms = registry.get<BGFXUniforms>(renderCommand.shaderProgram);

		for (auto&& [uniformName, uniformValueAny] : renderCommand.uniformData) {
			if (!bgfxUniforms.uniforms.contains(uniformName)) {
				return;
			}

			bgfx::UniformHandle uniformHandle = bgfxUniforms.uniforms.at(uniformName);

			const auto& uniformTypeId{ uniformValueAny.getTypeId() };
			if (uniformTypeId == Core::TypeId::get<entt::entity>()) {
				entt::entity uniformValueEntity = *static_cast<entt::entity*>(uniformValueAny.getInstance());
				if (registry.all_of<BGFXTexture>(uniformValueEntity)) {
					BGFXTexture& texture{ registry.get<BGFXTexture>(uniformValueEntity) };
					bgfx::setTexture(0, uniformHandle, texture.textureHandle);
				} else {
					return;
				}

			} else if (uniformTypeId == Core::TypeId::get<glm::vec4>()) {
				glm::vec4* uniformColour = static_cast<glm::vec4*>(uniformValueAny.getInstance());
				bgfx::setUniform(uniformHandle, uniformColour);
			} else {
				return;
			}
		}

		float mtx[16];
		bx::mtxIdentity(mtx);
		bgfx::setTransform(mtx);

		bgfx::setVertexBuffer(0, &transientVertexBuffer, 0, renderCommand.vertexCount);
		bgfx::setIndexBuffer(&transientIndexBuffer, 0, renderCommand.indexCount);

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

		// Submit primitive for rendering to view 0.
		bgfx::submit(renderCommand.renderPass, bgfxProgram.programHandle);
	}

	std::optional<std::string> getPlatformShaderFolder(bgfx::RendererType::Enum rendererType) {
		switch (rendererType) {
			case bgfx::RendererType::Direct3D11:
				return "dx11";
			case bgfx::RendererType::OpenGLES:
				return "essl";
			case bgfx::RendererType::OpenGL:
				return "glsl";
			case bgfx::RendererType::Vulkan:
				return "spirv";
			default:
				return std::nullopt;
		}
	}

	ShaderPlatformInfo getShaderPlatformInfo() {
		ShaderPlatformInfo shaderPlatformInfo{};
		shaderPlatformInfo.shadersFolderName = getPlatformShaderFolder(bgfx::getRendererType());
		return shaderPlatformInfo;
	}

	void destroyTexture(entt::registry& registry, const entt::entity entity) {
		if (!registry.all_of<BGFXTexture>(entity)) {
			return;
		}

		auto& texture{ registry.get<BGFXTexture>(entity) };
		if (!bgfx::isValid(texture.textureHandle)) {
			return;
		}

		bgfx::destroy(texture.textureHandle);
		texture.textureHandle = BGFX_INVALID_HANDLE;
	}

	void destroyShader(entt::registry& registry, const entt::entity entity) {
		if (!registry.all_of<BGFXShader>(entity)) {
			return;
		}

		auto& shader{ registry.get<BGFXShader>(entity) };
		if (!bgfx::isValid(shader.shaderHandle)) {
			return;
		}

		bgfx::destroy(shader.shaderHandle);
		shader.shaderHandle = BGFX_INVALID_HANDLE;
	}

	void destroyShaderProgram(entt::registry& registry, const entt::entity entity) {
		if (!registry.all_of<BGFXProgram>(entity)) {
			return;
		}

		auto& program{ registry.get<BGFXProgram>(entity) };
		if (!bgfx::isValid(program.programHandle)) {
			return;
		}

		bgfx::destroy(program.programHandle);
		program.programHandle = BGFX_INVALID_HANDLE;
	}



} // namespace Gfx::BGFXSystemInternal

namespace Gfx {

	void BGFXSystem::onWindowInternalCreated(entt::registry& registry, entt::entity entity) {
		if (!registry.view<const BGFXContext>().empty()) {
			return;
		}

		auto&& [nativeWindowHandles, window] = registry.get<Core::NativeWindowHandles, Core::Window>(entity);

		if (!nativeWindowHandles.windowHandle) {
			return;
		}

		using namespace BGFXSystemInternal;

		entt::entity contextEntity = registry.create();
		registry.emplace<BGFXContext>(contextEntity);
		auto& callbacks = registry.emplace<BGFXCallbacks>(contextEntity, registry, contextEntity);

		bgfx::Init initData;
		initData.platformData.nwh = nativeWindowHandles.windowHandle;
		initData.platformData.ndt = nativeWindowHandles.displayHandle;
		initData.resolution.width = window.width;
		initData.resolution.height = window.height;
		initData.callback = &callbacks;

		if (!bgfx::init(initData)) {
			registry.destroy(contextEntity);
			return;
		}

		registry.emplace<ShaderPlatformInfo>(contextEntity, getShaderPlatformInfo());


		registry.on_destroy<Image>()
			.connect<&destroyTexture>();

		registry.on_destroy<ShaderDescriptor>()
			.connect<&destroyShader>();

		registry.on_destroy<ShaderProgram>()
			.connect<&destroyShaderProgram>();
	}

	BGFXSystem::BGFXSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		initSystem(mRegistry);

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	BGFXSystem::~BGFXSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
		destroySystem(mRegistry);
	}

	void BGFXSystem::initSystem(entt::registry& registry) {
		registry.on_construct<Core::NativeWindowHandles>().connect<&BGFXSystem::onWindowInternalCreated>(this);
	}

	void BGFXSystem::destroySystem(entt::registry& registry) {
		if (registry.view<const BGFXContext>().empty()) {
			return;
		}

		registry.view<BGFXContext>().each([this](BGFXContext& context) { _destroyTrackedTextures(context); });

		bgfx::shutdown();
	}

	void BGFXSystem::tickSystem(entt::registry& registry) {
		using namespace BGFXSystemInternal;

		auto bgfxContextView = registry.view<BGFXContext>();
		if (bgfxContextView.empty()) {
			return;
		}

		auto bgfxContextEntity{ bgfxContextView.front() };
		auto& bgfxContext{ registry.get<BGFXContext>(bgfxContextEntity) };

		registry.view<const ShaderProgram>(entt::exclude<BGFXUniforms, Core::ProcessError>)
			.each([&registry, &bgfxContext](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateUniforms(bgfxContext, registry, entity, shaderProgram);
			});

		registry
			.view<const ShaderDescriptor, const Core::RawDataResource>(entt::exclude<Core::FileLoadRequest, Core::ProcessError, BGFXShader>)
			.each([&registry, &bgfxContext](entt::entity entity, const Core::RawDataResource& rawDataResource) {
				_tryCreateShader(bgfxContext, registry, entity, rawDataResource);
			});

		registry.view<const ShaderProgram>(entt::exclude<BGFXProgram, Core::ProcessError>)
			.each([&registry, &bgfxContext](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateShaderProgram(bgfxContext, registry, entity, shaderProgram);
			});

		registry.view<const ShaderProgram>(entt::exclude<BGFXVertexLayout, Core::ProcessError>)
			.each([&registry, &bgfxContext](entt::entity entity, const ShaderProgram& shaderProgram) {
				_tryCreateVertexLayout(bgfxContext, registry, entity, shaderProgram);
			});

		registry.view<const Image>(entt::exclude<BGFXTexture, Core::ProcessError>)
			.each([&registry, &bgfxContext](entt::entity entity, const Image& image) {
				_tryCreateTexture(bgfxContext, registry, entity, image);
			});

		bgfx::ViewId currentViewId{ 0 };
		registry.view<Viewport>().each([&currentViewId, this](const Viewport& viewport) {
			const bgfx::ViewId viewId = currentViewId++;
			constexpr float width = 1360.0f;
			constexpr float height = 768.0f;

			const float left = viewport.offset.x * width;
			const float right = viewport.dimensions.x * width;
			const float top = viewport.offset.y * height;
			const float bottom = viewport.dimensions.y * height;

			bgfx::setViewRect(0, left, top, right - left, bottom - top);
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);

			bgfx::touch(viewId);
			bgfx::dbgTextClear();
			bgfx::setDebug(BGFX_DEBUG_TEXT);
		});

		std::unordered_map<bgfx::ViewId, std::vector<RenderCommand>> commandBuckets;
		registry.view<RenderCommand>().each([&commandBuckets, &registry](const RenderCommand& renderCommand) {
			commandBuckets[renderCommand.renderPass].emplace_back(renderCommand);
		});

		for (auto&& [viewId, renderCommands] : commandBuckets) {
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

		registry.view<BGFXDrawCallback>().each(
			[&registry](const BGFXDrawCallback& drawCallback) { drawCallback.drawCallback(registry); });

		bgfx::frame();

		registry.view<BGFXContext>().each(
			[this, &registry](BGFXContext& context) { _destroyDroppedTextures(registry, context); });
	}

	void BGFXSystem::_destroyDroppedTextures(const entt::registry& registry, BGFXContext& context) {

	}

	void BGFXSystem::_destroyTrackedTextures(BGFXContext& context) const {

	}

	void BGFXSystem::_tryCreateShader(
		BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity,
		const Core::RawDataResource& rawDataResource) {
		using namespace BGFXSystemInternal;

		const bgfx::Memory* mem = bgfx::copy(rawDataResource.rawData.data(), rawDataResource.size);
		const bgfx::ShaderHandle shaderHandle = bgfx::createShader(mem);
		if (!bgfx::isValid(shaderHandle)) {
			Core::addProcessError(registry, entity, "Failed to create shader.");
			return;
		}

		registry.emplace<BGFXShader>(entity, shaderHandle);
	}

	void BGFXSystem::_tryCreateShaderProgram(
		BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {
		using namespace BGFXSystemInternal;

		const auto& [ vertexShaderEntity, vertexShader] = Core::getResourceAndEntity<BGFXShader>(registry, shaderProgram.vertexShader);
		const auto& [ fragmentShaderEntity, fragmentShader] = Core::getResourceAndEntity<BGFXShader>(registry, shaderProgram.fragmentShader);

		if (!vertexShader || !fragmentShader) {
			const bool shouldRetry = !Core::hasHaltedProcessError(registry, vertexShaderEntity) &&
									 !Core::hasHaltedProcessError(registry, fragmentShaderEntity);

			Core::addProcessError(registry, entity, shouldRetry);

			return;
		}

		if (!bgfx::isValid(vertexShader->shaderHandle) ||
			!bgfx::isValid(fragmentShader->shaderHandle)) {
			Core::addProcessError(registry, entity, "Invalid shader handles passed to program creation.");
			return;
		}

		bgfx::ProgramHandle program = bgfx::createProgram(vertexShader->shaderHandle, fragmentShader->shaderHandle);
		if (!bgfx::isValid(program)) {
			Core::addProcessError(registry, entity, "Failed to create shader program handle.");
			return;
		}

		registry.emplace<BGFXProgram>(entity, program);
	}

	void BGFXSystem::_tryCreateVertexLayout(
		BGFXContext&, entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {

		using namespace BGFXSystemInternal;
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
				default:
					Core::addProcessError(registry, entity, "Unhandled vertex attribute type.");
					vertexLayout.end();
					return;
			}

			bgfx::AttribType::Enum attributeType{};
			if (inputAttribute.typeId == Core::TypeId::get<float>()) {
				attributeType = bgfx::AttribType::Float;
			} else {
				Core::addProcessError(registry, entity, "Unhandled attribute type.");
				vertexLayout.end();
				return;
			}

			vertexLayout.add(attribute, inputAttribute.count, attributeType);
		}

		vertexLayout.end();

		registry.emplace<BGFXVertexLayout>(entity, vertexLayout);
	}

	void BGFXSystem::_tryCreateUniforms(
		BGFXContext&, entt::registry& registry, entt::entity entity, const ShaderProgram& shaderProgram) {

		using namespace BGFXSystemInternal;
		BGFXUniforms bgfxUniforms{};
		for (auto&& uniform : shaderProgram.uniforms) {
			if (bgfxUniforms.uniforms.contains(uniform.name)) {
				Core::addProcessError(registry, entity, "Uniform with duplicate name '{}' encountered.", uniform.name);
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
					Core::addProcessError(registry, entity, "Unhandled type for uniform '{}'.", uniform.name);
					return;
			}

			bgfx::UniformHandle uniformHandle{ bgfx::createUniform(uniform.name.c_str(), uniformType) };
			if (!bgfx::isValid(uniformHandle)) {
				Core::addProcessError(registry, entity, "Failed to create BGFX uniform '{}'.", uniform.name);
				return;
			}

			bgfxUniforms.uniforms.emplace(uniform.name, uniformHandle);
		}

		registry.emplace<BGFXUniforms>(entity, std::move(bgfxUniforms));
	}

	void BGFXSystem::_tryCreateTexture(
		BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity, const Image& imageResource) {
		using namespace BGFXSystemInternal;

		const bgfx::Memory* mem = bgfx::copy(imageResource.image.data(), imageResource.image.size());
		bgfx::TextureHandle texture = bgfx::createTexture2D(
			imageResource.width, imageResource.height, imageResource.numMips > 1, imageResource.numLayers,
			static_cast<bgfx::TextureFormat::Enum>(imageResource.format), BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, mem);

		if (!bgfx::isValid(texture)) {
			Core::addProcessError(registry, entity, "Failed to create BGFX texture.");
		}

		registry.emplace<BGFXTexture>(entity, texture);
	}

} // namespace Gfx
