module;

#include <filesystem>
#include <fstream>

#include <bx/platform.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if GLFW_VERSION_MINOR < 2
#error "GLFW 3.2 or later is required"
#endif // GLFW_VERSION_MINOR < 2

#if BX_PLATFORM_LINUX
#if ENTRY_CONFIG_USE_WAYLAND
#include <wayland-egl.h>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#else
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <GLFW/glfw3native.h>
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#include <GLFW/glfw3native.h>
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif //

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <entt/entt.hpp>

module Gfx.BGFXSystem;

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ImageResource;
import Core.RawDataResource;
import Core.ResourceHandle;
import Core.Spatial;
import Core.Window;
import Core.WindowInternal;
import Gfx.BGFXContext;
import Gfx.BGFXDrawCallback;
import Gfx.MaterialResource;
import Gfx.RenderObject;
import Gfx.ShaderDescriptor;
import Gfx.ShaderProgramDescriptor;
import Gfx.ShaderProgramResource;
import Gfx.TextureCoordinates;

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

	struct BGFXQuadMesh {
		bgfx::VertexBufferHandle vertexBuffer{};
		bgfx::IndexBufferHandle indexBuffer{};
		bgfx::UniformHandle textureUniformHandle{};
	};

	struct QuadBufferData {
		bgfx::TransientVertexBuffer transientVertexBuffer{};
		bgfx::TransientIndexBuffer transientIndexBuffer{};
		uint32_t quadCount{};
		uint32_t maxVertexCount{};
		uint32_t maxIndexCount{};
	};

	struct PosColorVertex {
		float x{};
		float y{};
		float z{};
		float u{};
		float v{};

		static void init() {
			layout.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.end();
		};

		static bgfx::VertexLayout layout;
	};

	bgfx::VertexLayout PosColorVertex::layout;

	std::optional<entt::entity> getMaterialResourceIfReadyToRender(const entt::registry& registry,
																   const entt::entity materialResourceHandle) {
		if (materialResourceHandle == entt::null || !registry.all_of<Core::ResourceHandle>(materialResourceHandle)) {
			return std::nullopt;
		}

		const auto& resourceHandle = registry.get<Core::ResourceHandle>(materialResourceHandle);

		const entt::entity materialResourceEntity = resourceHandle.mResourceEntity;
		if (materialResourceEntity == entt::null || !registry.all_of<MaterialResource>(materialResourceEntity)) {
			return std::nullopt;
		}

		const auto& materialResource = registry.get<MaterialResource>(materialResourceEntity);

		if (materialResource.textureImageResource == entt::null ||
			!registry.all_of<Core::ResourceHandle>(materialResource.textureImageResource)) {

			return std::nullopt;
		}

		if (materialResource.shaderProgramResource == entt::null ||
			!registry.all_of<Core::ResourceHandle>(materialResource.shaderProgramResource)) {

			return std::nullopt;
		}

		const auto& textureImageResourceHandle =
				registry.get<Core::ResourceHandle>(materialResource.textureImageResource);

		if (textureImageResourceHandle.mResourceEntity == entt::null ||
			!registry.all_of<BGFXTexture>(textureImageResourceHandle.mResourceEntity)) {

			return std::nullopt;
		}

		const auto& shaderProgramResourceHandle =
				registry.get<Core::ResourceHandle>(materialResource.shaderProgramResource);

		if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
			!registry.all_of<BGFXProgram>(shaderProgramResourceHandle.mResourceEntity)) {

			return std::nullopt;
		}

		return materialResourceEntity;
	}

	using PerMaterialBufferMap = std::unordered_map<entt::entity, QuadBufferData>;

	PerMaterialBufferMap createPerMaterialBuffers(entt::registry& registry) {
		PerMaterialBufferMap materialBuffers;

		registry.view<RenderObject>().each([&registry, &materialBuffers](const RenderObject& renderObject) {
			auto materialResourceEntity = getMaterialResourceIfReadyToRender(registry, renderObject.materialResource);
			if (!materialResourceEntity) {
				return;
			}

			const MaterialResource& materialResource = registry.get<MaterialResource>(*materialResourceEntity);
			if (renderObject.currentSpriteFrame >= materialResource.spriteFrames.size()) {
				return;
			}

			constexpr uint32_t maxVertices = (32 << 10);
			constexpr uint32_t maxIndices = (32 << 10);
			if (!materialBuffers.contains(*materialResourceEntity)) {
				if (bgfx::getAvailTransientVertexBuffer(maxVertices, PosColorVertex::layout) < maxVertices ||
					bgfx::getAvailTransientIndexBuffer(maxIndices) < maxIndices) {
					return;
				}

				bgfx::TransientVertexBuffer transientVertexBuffer{};
				bgfx::allocTransientVertexBuffer(&transientVertexBuffer, maxVertices, PosColorVertex::layout);

				bgfx::TransientIndexBuffer transientIndexBuffer{};
				bgfx::allocTransientIndexBuffer(&transientIndexBuffer, maxIndices);

				materialBuffers.emplace(
						*materialResourceEntity,
						QuadBufferData{transientVertexBuffer, transientIndexBuffer, 0, maxVertices, maxIndices});
			}
		});

		return materialBuffers;
	}

	void extractRenderObjectToMaterialBuffer(entt::registry& registry, QuadBufferData& bufferData,
											 const Core::Spatial& spatial, const RenderObject& renderObject,
											 const MaterialResource& materialResource) {

		if (renderObject.currentSpriteFrame >= materialResource.spriteFrames.size()) {
			return;
		}

		const auto& frameCoords{materialResource.spriteFrames[renderObject.currentSpriteFrame]};
		const uint32_t startVertex = bufferData.quadCount * 4;
		const uint32_t startIndex = bufferData.quadCount * 6;
		if (startVertex + 4 >= bufferData.maxVertexCount || startIndex + 6 >= bufferData.maxIndexCount) {
			return;
		}

		const float quadWidth = (frameCoords.x1 - frameCoords.x0) * 2.0f; // / materialResource.width;
		const float quadHeight = (frameCoords.y1 - frameCoords.y0) * 2.0f; // / materialResource.height;

		const TextureCoordinates factoredCoords{
				frameCoords.x0 / materialResource.width, frameCoords.y0 / materialResource.height,
				frameCoords.x1 / materialResource.width, frameCoords.y1 / materialResource.height};

		PosColorVertex* vertexHead =
				reinterpret_cast<PosColorVertex*>(bufferData.transientVertexBuffer.data) + startVertex;
		vertexHead[0] = {spatial.x, spatial.y, spatial.z, factoredCoords.x0, factoredCoords.y0};
		vertexHead[1] = {spatial.x, spatial.y + quadHeight, spatial.z, factoredCoords.x0, factoredCoords.y1};
		vertexHead[2] = {spatial.x + quadWidth, spatial.y + quadHeight, spatial.z, factoredCoords.x1,
						 factoredCoords.y1};
		vertexHead[3] = {spatial.x + quadWidth, spatial.y, spatial.z, factoredCoords.x1, factoredCoords.y0};

		uint16_t* indexHead = reinterpret_cast<uint16_t*>(bufferData.transientIndexBuffer.data) + startIndex;

		// 0, 1, 3,
		indexHead[0] = startVertex;
		indexHead[1] = startVertex + 1;
		indexHead[2] = startVertex + 3;
		// 3, 1, 2
		indexHead[3] = startVertex + 3;
		indexHead[4] = startVertex + 1;
		indexHead[5] = startVertex + 2;

		++bufferData.quadCount;
	}

	void extractRenderObjectsToPerMaterialBuffers(entt::registry& registry, PerMaterialBufferMap& materialBuffers) {
		registry.view<Core::Spatial, RenderObject>().each([&registry, &materialBuffers](auto& spatial,
																						auto& renderObject) {
			auto materialResourceEntity = getMaterialResourceIfReadyToRender(registry, renderObject.materialResource);
			if (!materialResourceEntity) {
				return;
			}

			const MaterialResource& materialResource = registry.get<MaterialResource>(*materialResourceEntity);
			auto& quadBufferData{materialBuffers[*materialResourceEntity]};

			extractRenderObjectToMaterialBuffer(registry, quadBufferData, spatial, renderObject, materialResource);
		});
	}

	void renderPerMaterialBuffers(entt::registry& registry, const PerMaterialBufferMap& materialBuffers,
								  bgfx::UniformHandle textureColourUniform, bgfx::UniformHandle alphaColourUniform) {

		for (const auto& [materialEntity, transientBufferData] : materialBuffers) {
			if (materialEntity == entt::null || !registry.all_of<MaterialResource>(materialEntity)) {
				continue;
			}

			const MaterialResource& materialResource = registry.get<MaterialResource>(materialEntity);
			if (!registry.all_of<Core::ResourceHandle>(materialResource.textureImageResource) ||
				!registry.all_of<Core::ResourceHandle>(materialResource.shaderProgramResource)) {
				return;
			}

			const auto& textureImageResourceHandle =
					registry.get<Core::ResourceHandle>(materialResource.textureImageResource);
			if (textureImageResourceHandle.mResourceEntity == entt::null ||
				!registry.all_of<BGFXTexture>(textureImageResourceHandle.mResourceEntity)) {
				return;
			}

			const auto& shaderProgramResourceHandle =
					registry.get<Core::ResourceHandle>(materialResource.shaderProgramResource);
			if (shaderProgramResourceHandle.mResourceEntity == entt::null ||
				!registry.all_of<BGFXProgram>(shaderProgramResourceHandle.mResourceEntity)) {
				return;
			}

			const auto& bgfxProgram = registry.get<BGFXProgram>(shaderProgramResourceHandle.mResourceEntity);
			const auto& bgfxTexture = registry.get<BGFXTexture>(textureImageResourceHandle.mResourceEntity);

			float mtx[16];
			bx::mtxIdentity(mtx);
			bgfx::setTransform(mtx);

			bgfx::setVertexBuffer(0, &transientBufferData.transientVertexBuffer, 0, transientBufferData.quadCount * 4);
			bgfx::setIndexBuffer(&transientBufferData.transientIndexBuffer, 0, transientBufferData.quadCount * 6);
			bgfx::setTexture(0, textureColourUniform, bgfxTexture.textureHandle);

			const auto& alphaColour = materialResource.alphaColour.value_or({255.0f, 255.0f, 255.0f, 255.0f});
			bgfx::setUniform(alphaColourUniform, &alphaColour);

			// Set render states.
			bgfx::setState(0 | BGFX_STATE_WRITE_RGB |
						   BGFX_STATE_WRITE_A
						   //| BGFX_STATE_WRITE_Z
						   //| BGFX_STATE_DEPTH_TEST_LESS
						   | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA) |
						   BGFX_STATE_BLEND_EQUATION_ADD
						   //| BGFX_STATE_MSAA
			);

			// Submit primitive for rendering to view 0.
			bgfx::submit(0, bgfxProgram.programHandle);
		}
	}

} // namespace Gfx::BGFXSystemInternal

namespace Gfx {

	void* glfwNativeWindowHandle(GLFWwindow* _window) {
#if BX_PLATFORM_LINUX
#if ENTRY_CONFIG_USE_WAYLAND
		wl_egl_window* win_impl = (wl_egl_window*) glfwGetWindowUserPointer(_window);
		if (!win_impl) {
			int width, height;
			glfwGetWindowSize(_window, &width, &height);
			struct wl_surface* surface = (struct wl_surface*) glfwGetWaylandWindow(_window);
			if (!surface)
				return nullptr;
			win_impl = wl_egl_window_create(surface, width, height);
			glfwSetWindowUserPointer(_window, (void*) (uintptr_t) win_impl);
		}
		return (void*) (uintptr_t) win_impl;
#else
		return reinterpret_cast<void*>(glfwGetX11Window(_window));
#endif
#elif BX_PLATFORM_OSX
		return glfwGetCocoaWindow(_window);
#elif BX_PLATFORM_WINDOWS
		return glfwGetWin32Window(_window);
#elif BX_PLATFORM_EMSCRIPTEN
		return nullptr;
#else
		static_assert(false, "Unhandled platform");
#endif
	}

	void* getNativeDisplayHandle() {
#if BX_PLATFORM_LINUX
#if ENTRY_CONFIG_USE_WAYLAND
		return glfwGetWaylandDisplay();
#else
		return glfwGetX11Display();
#endif // ENTRY_CONFIG_USE_WAYLAND
#else
		return NULL;
#endif // BX_PLATFORM_*
	}

	void BGFXSystem::onWindowInternalCreated(entt::registry& registry, entt::entity entity) {
		if (!registry.view<const BGFXContext>().empty()) {
			return;
		}

		auto&& [windowInternal, window] = registry.get<Core::WindowInternal, Core::Window>(entity);

		void* nativeWindowHandle = glfwNativeWindowHandle(windowInternal.window);
		if (!nativeWindowHandle) {
			return;
		}

		bgfx::Init initData;
		initData.platformData.nwh = nativeWindowHandle;
		initData.platformData.ndt = getNativeDisplayHandle();
		initData.resolution.width = window.width;
		initData.resolution.height = window.height;

		if (!bgfx::init(initData)) {
			return;
		}

		entt::entity contextEntity = registry.create();
		registry.emplace<BGFXContext>(contextEntity);

		// Create vertex stream declaration.
		BGFXSystemInternal::PosColorVertex::init();

		mTexColourUniformHandle = bgfx::createUniform("s_texColour", bgfx::UniformType::Sampler);
		mAlphaColourUniformHandle = bgfx::createUniform("u_alphaColour", bgfx::UniformType::Vec4);
	}

	BGFXSystem::BGFXSystem(Core::EnTTRegistry& registry) : mRegistry{registry} {}

	BGFXSystem::~BGFXSystem() = default;

	void BGFXSystem::initSystem(entt::registry& registry) {
		registry.on_construct<Core::WindowInternal>().connect<&BGFXSystem::onWindowInternalCreated>(this);
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

		auto bgfxContextEntity{bgfxContextView.front()};
		auto& bgfxContext{registry.get<BGFXContext>(bgfxContextEntity)};

		registry.view<const ShaderDescriptor, const Core::RawDataResource>(
						entt::exclude<Core::FileLoadRequest, BGFXShader>)
				.each([&registry, &bgfxContext](entt::entity entity, const Core::RawDataResource& rawDataResource) {
					_tryCreateShader(bgfxContext, registry, entity, rawDataResource);
				});

		registry.view<const ShaderProgramResource>(entt::exclude<BGFXProgram>)
				.each([&registry, &bgfxContext](entt::entity entity,
												const ShaderProgramResource& shaderProgramResource) {
					_tryCreateShaderProgram(bgfxContext, registry, entity, shaderProgramResource);
				});

		registry.view<const Core::ImageResource>(entt::exclude<BGFXTexture>)
				.each([&registry, &bgfxContext](entt::entity entity, const Core::ImageResource& imageResource) {
					_tryCreateTexture(bgfxContext, registry, entity, imageResource);
				});

		const bgfx::ViewId viewId{0};
		bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
		bgfx::setViewRect(viewId, 0, 0, bgfx::BackbufferRatio::Equal);

		bgfx::touch(viewId);
		bgfx::dbgTextClear();
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		float view[16];
		bx::mtxIdentity(view);

		float proj[16];
		bx::mtxOrtho(proj, 0.0f, 1360.0f, 768.0f, 0.0f, 0.0f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		// TODO: pre-sort render objects by material and THEN create and submit buffers one by one.
		using namespace BGFXSystemInternal;
		auto materialBuffers = createPerMaterialBuffers(registry);
		extractRenderObjectsToPerMaterialBuffers(registry, materialBuffers);
		renderPerMaterialBuffers(registry, materialBuffers, mTexColourUniformHandle, mAlphaColourUniformHandle);

		registry.view<BGFXDrawCallback>().each(
				[&registry](const BGFXDrawCallback& drawCallback) { drawCallback.drawCallback(registry); });

		bgfx::frame();

		registry.view<BGFXContext>().each(
				[this, &registry](BGFXContext& context) { _destroyDroppedTextures(registry, context); });
	}

	void BGFXSystem::_destroyDroppedTextures(const entt::registry& registry, BGFXContext& context) {
		using namespace BGFXSystemInternal;
		for (auto it = context.trackedTextureHandles.begin(); it != context.trackedTextureHandles.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->first) && registry.all_of<BGFXTexture>(currentIt->first)) {
				continue;
			}

			bgfx::destroy(currentIt->second);

			context.trackedTextureHandles.erase(currentIt);
		}

		for (auto it = context.trackedShaderHandles.begin(); it != context.trackedShaderHandles.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->first) && registry.all_of<BGFXShader>(currentIt->first)) {
				continue;
			}

			bgfx::destroy(currentIt->second);

			context.trackedShaderHandles.erase(currentIt);
		}

		for (auto it = context.trackedShaderProgramHandles.begin(); it != context.trackedShaderProgramHandles.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->first) && registry.all_of<BGFXProgram>(currentIt->first)) {
				continue;
			}

			bgfx::destroy(currentIt->second);

			context.trackedShaderProgramHandles.erase(currentIt);
		}
	}

	void BGFXSystem::_destroyTrackedTextures(BGFXContext& context) const {
		for (const auto textureHandle : context.trackedTextureHandles | std::views::values) {
			bgfx::destroy(textureHandle);
		}

		for (const auto shaderHandle : context.trackedShaderHandles | std::views::values) {
			bgfx::destroy(shaderHandle);
		}

		for (const auto shaderProgramHandle : context.trackedShaderProgramHandles | std::views::values) {
			bgfx::destroy(shaderProgramHandle);
		}

		bgfx::destroy(mTexColourUniformHandle);
		bgfx::destroy(mAlphaColourUniformHandle);

		context.trackedTextureHandles.clear();
		context.trackedShaderHandles.clear();
		context.trackedShaderProgramHandles.clear();
	}

	void BGFXSystem::_tryCreateShader(BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity,
									  const Core::RawDataResource& rawDataResource) {
		using namespace BGFXSystemInternal;

		const bgfx::Memory* mem = bgfx::copy(rawDataResource.rawData.data(), rawDataResource.size);
		const bgfx::ShaderHandle shaderHandle = bgfx::createShader(mem);
		if (shaderHandle.idx == bgfx::kInvalidHandle) {
			return;
		}

		registry.emplace<BGFXShader>(entity, shaderHandle);
		bgfxContext.trackedShaderHandles.emplace_back(entity, shaderHandle);
	}

	void BGFXSystem::_tryCreateShaderProgram(BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity,
											 const ShaderProgramResource& shaderProgramResource) {
		using namespace BGFXSystemInternal;

		if (shaderProgramResource.vertexShaderResource == entt::null ||
			shaderProgramResource.fragmentShaderResource == entt::null) {
			return;
		}

		if (!registry.all_of<Core::ResourceHandle>(shaderProgramResource.vertexShaderResource) ||
			!registry.all_of<Core::ResourceHandle>(shaderProgramResource.fragmentShaderResource)) {
			return;
		}

		const auto& vsResourceHandle{registry.get<Core::ResourceHandle>(shaderProgramResource.vertexShaderResource)};
		const auto& fsResourceHandle{registry.get<Core::ResourceHandle>(shaderProgramResource.fragmentShaderResource)};

		if (!registry.all_of<BGFXShader>(vsResourceHandle.mResourceEntity) ||
			!registry.all_of<BGFXShader>(fsResourceHandle.mResourceEntity)) {
			return;
		}

		const auto& vertexShader = registry.get<BGFXShader>(vsResourceHandle.mResourceEntity);
		const auto& fragmentShader = registry.get<BGFXShader>(fsResourceHandle.mResourceEntity);

		if (vertexShader.shaderHandle.idx == bgfx::kInvalidHandle ||
			fragmentShader.shaderHandle.idx == bgfx::kInvalidHandle) {
			return;
		}

		bgfx::ProgramHandle program = bgfx::createProgram(vertexShader.shaderHandle, fragmentShader.shaderHandle);
		if (program.idx == bgfx::kInvalidHandle) {
			return;
		}

		registry.emplace<BGFXProgram>(entity, program);
		bgfxContext.trackedShaderProgramHandles.emplace_back(entity, program);
	}

	void BGFXSystem::_tryCreateTexture(BGFXContext& bgfxContext, entt::registry& registry, entt::entity entity,
									   const Core::ImageResource& imageResource) {
		using namespace BGFXSystemInternal;

		const bgfx::Memory* mem = bgfx::copy(imageResource.image.data(), imageResource.image.size());
		bgfx::TextureHandle texture = bgfx::createTexture2D(
				imageResource.width, imageResource.height, imageResource.numMips > 1, imageResource.numLayers,
				static_cast<bgfx::TextureFormat::Enum>(imageResource.format), BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
				mem);

		registry.emplace<BGFXTexture>(entity, texture);
		bgfxContext.trackedTextureHandles.emplace_back(entity, texture);
	}

} // namespace Gfx
