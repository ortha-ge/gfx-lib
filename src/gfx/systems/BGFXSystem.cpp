module;

#include <filesystem>
#include <fstream>

#include <bx/platform.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if GLFW_VERSION_MINOR < 2
#	error "GLFW 3.2 or later is required"
#endif // GLFW_VERSION_MINOR < 2

#if BX_PLATFORM_LINUX
#	if ENTRY_CONFIG_USE_WAYLAND
#		include <wayland-egl.h>
#		define GLFW_EXPOSE_NATIVE_WAYLAND
#	else
#		define GLFW_EXPOSE_NATIVE_X11
#		define GLFW_EXPOSE_NATIVE_GLX
#	endif
#	include <GLFW/glfw3native.h>
#elif BX_PLATFORM_OSX
#	define GLFW_EXPOSE_NATIVE_COCOA
#	define GLFW_EXPOSE_NATIVE_NSGL
#	include <GLFW/glfw3native.h>
#elif BX_PLATFORM_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#	include <GLFW/glfw3native.h>
#endif //

#include <entt/entt.hpp>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

module Gfx.BGFXSystem;

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ImageResource;
import Core.RawDataResource;
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

namespace Gfx {

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

	static void* glfwNativeWindowHandle(GLFWwindow* _window)
	{
#	if BX_PLATFORM_LINUX
# 		if ENTRY_CONFIG_USE_WAYLAND
		wl_egl_window *win_impl = (wl_egl_window*)glfwGetWindowUserPointer(_window);
		if(!win_impl)
		{
			int width, height;
			glfwGetWindowSize(_window, &width, &height);
			struct wl_surface* surface = (struct wl_surface*)glfwGetWaylandWindow(_window);
			if(!surface)
				return nullptr;
			win_impl = wl_egl_window_create(surface, width, height);
			glfwSetWindowUserPointer(_window, (void*)(uintptr_t)win_impl);
		}
		return (void*)(uintptr_t)win_impl;
#		else
		return (void*)(uintptr_t)glfwGetX11Window(_window);
#		endif
#	elif BX_PLATFORM_OSX
		return glfwGetCocoaWindow(_window);
#	elif BX_PLATFORM_WINDOWS
		return glfwGetWin32Window(_window);
#	elif BX_PLATFORM_EMSCRIPTEN
		return nullptr;
#	else
		static_assert(false, "Unhandled platform");
#	endif
	}

	void* getNativeDisplayHandle()
	{
#	if BX_PLATFORM_LINUX
#		if ENTRY_CONFIG_USE_WAYLAND
		return glfwGetWaylandDisplay();
#		else
		return glfwGetX11Display();
#		endif // ENTRY_CONFIG_USE_WAYLAND
#	else
		return NULL;
#	endif // BX_PLATFORM_*
	}

	struct PosColorVertex
	{
		float x{};
		float y{};
		float z{};
		float u{};
		float v{};

		static void init()
		{
			layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		};

		static bgfx::VertexLayout layout;
	};

	bgfx::VertexLayout PosColorVertex::layout;

	bgfx::ShaderHandle loadShader(std::string_view shaderFilePath) {
		uintmax_t fileSize = std::filesystem::file_size(shaderFilePath);

		std::vector<uint8_t> rawData;
		rawData.resize(fileSize);

		std::ifstream fileStream(std::string{ shaderFilePath }, std::ios::binary);
		fileStream.read(reinterpret_cast<char*>(rawData.data()), fileSize);

		const bgfx::Memory* mem = bgfx::copy(rawData.data(), rawData.size());

		return bgfx::createShader(mem);
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

		// Quad Buffers

		// Create vertex stream declaration.
		PosColorVertex::init();

		mTexColourUniformHandle = bgfx::createUniform("s_texColour",  bgfx::UniformType::Sampler);
		mAlphaColourUniformHandle = bgfx::createUniform("u_alphaColour", bgfx::UniformType::Vec4);
	}

	BGFXSystem::BGFXSystem() {
	}

	void BGFXSystem::initSystem(entt::registry& registry) {
		registry.on_construct<Core::WindowInternal>().connect<&BGFXSystem::onWindowInternalCreated>(this);
	}

	void BGFXSystem::destroySystem(entt::registry& registry) {
		if (registry.view<const BGFXContext>().empty()) {
			return;
		}

		bgfx::shutdown();
	}

	void BGFXSystem::tickSystem(entt::registry& registry) {
		if (registry.view<const BGFXContext>().empty()) {
			return;
		}

		auto createShaderView = registry.view<const ShaderDescriptor, const Core::RawDataResource>(entt::exclude<Core::FileLoadRequest, BGFXShader>);
		createShaderView.each([&registry](entt::entity entity, const Core::RawDataResource& rawDataResource) {
			const bgfx::Memory* mem = bgfx::copy(rawDataResource.rawData.data(), rawDataResource.size);
			const bgfx::ShaderHandle shaderHandle = bgfx::createShader(mem);
			if (shaderHandle.idx == bgfx::kInvalidHandle) {
				return;
			}

			registry.emplace<BGFXShader>(entity, shaderHandle);
		});

		// Need to figure out referencing
		auto createProgramView = registry.view<const ShaderProgramResource>(entt::exclude<BGFXProgram>);
		createProgramView.each([&registry](entt::entity entity, const ShaderProgramResource& shaderProgramDescriptor) {
			if (shaderProgramDescriptor.vertexShaderResource == entt::null ||
				shaderProgramDescriptor.fragmentShaderResource == entt::null) {
				return;
			}

			const BGFXShader* vertexShader = registry.try_get<BGFXShader>(shaderProgramDescriptor.vertexShaderResource);
			const BGFXShader* fragmentShader = registry.try_get<BGFXShader>(shaderProgramDescriptor.fragmentShaderResource);

			if (vertexShader == nullptr || fragmentShader == nullptr) {
				return;
			}

			if (vertexShader->shaderHandle.idx == bgfx::kInvalidHandle || fragmentShader->shaderHandle.idx == bgfx::kInvalidHandle) {
				return;
			}

			bgfx::ProgramHandle program = bgfx::createProgram(vertexShader->shaderHandle, fragmentShader->shaderHandle);
			if (program.idx == bgfx::kInvalidHandle) {
				return;
			}

			registry.emplace<BGFXProgram>(entity, program);
		});

		auto createTexturesView = registry.view<const Core::ImageResource>(entt::exclude<BGFXTexture>);
		createTexturesView.each([&registry](entt::entity entity, const Core::ImageResource& imageResource) {
			const bgfx::Memory* mem = bgfx::copy(imageResource.image.data(), imageResource.image.size());
			bgfx::TextureHandle texture = bgfx::createTexture2D(imageResource.width,
				imageResource.height,
				imageResource.numMips > 1,
				imageResource.numLayers,
				static_cast<bgfx::TextureFormat::Enum>(imageResource.format),
				BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
				mem);
			registry.emplace<BGFXTexture>(entity, texture);
		});

		const bgfx::ViewId viewId{ 0 };
		bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
		bgfx::setViewRect(viewId, 0, 0, bgfx::BackbufferRatio::Equal);

		bgfx::touch(viewId);
		bgfx::dbgTextClear();
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		static float cameraX = 0.0f;

		float view[16];
		bx::mtxIdentity(view);
		bx::mtxTranslate(view, cameraX, 0.0f, 0.0f);

		//cameraX += 0.01f;

		float proj[16];
		bx::mtxOrtho(proj, 0.0f, 1360.0f, 768.0f, 0.0f, 0.0f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		auto renderObjectsView = registry.view<const Core::Spatial, const RenderObject>();

		struct QuadBufferData {
			bgfx::TransientVertexBuffer transientVertexBuffer{};
			bgfx::TransientIndexBuffer transientIndexBuffer{};
			uint32_t quadCount{};
		};

		std::unordered_map<entt::entity, QuadBufferData> perMaterialTransientBuffers{};
		renderObjectsView.each([&registry, &perMaterialTransientBuffers](entt::entity entity, const Core::Spatial& spatial, const RenderObject& renderObject) {
			const entt::entity materialEntity = renderObject.materialResource;
			if (materialEntity == entt::null || !registry.all_of<MaterialResource>(materialEntity)) {
				return;
			}

			const MaterialResource& materialResource = registry.get<MaterialResource>(materialEntity);
			if (!registry.all_of<BGFXProgram>(materialResource.shaderProgramResource) ||
				!registry.all_of<BGFXTexture>(materialResource.textureImageResource)) {
				return;
			}

			if (renderObject.currentSpriteFrame >= materialResource.spriteFrames.size()) {
				return;
			}

			const auto& frameCoords{ materialResource.spriteFrames[renderObject.currentSpriteFrame] };

			constexpr uint32_t maxVertices = (32 << 10);
			constexpr uint32_t maxIndices = (32 << 10);
			if (!perMaterialTransientBuffers.contains(materialEntity)) {
				if (bgfx::getAvailTransientVertexBuffer(maxVertices, PosColorVertex::layout) < maxVertices ||
					bgfx::getAvailTransientIndexBuffer(maxIndices) < maxIndices) {
					return;
				}

				bgfx::TransientVertexBuffer transientVertexBuffer{};
				bgfx::allocTransientVertexBuffer(&transientVertexBuffer, maxVertices, PosColorVertex::layout);

				bgfx::TransientIndexBuffer transientIndexBuffer{};
				bgfx::allocTransientIndexBuffer(&transientIndexBuffer, maxIndices);

				perMaterialTransientBuffers.emplace(materialEntity, QuadBufferData{ transientVertexBuffer, transientIndexBuffer });
			}

			auto& transientBuffer = perMaterialTransientBuffers[materialEntity];
			const uint32_t startVertex = transientBuffer.quadCount * 4;
			const uint32_t startIndex = transientBuffer.quadCount * 6;
			if (startVertex + 4 >= maxVertices || startIndex + 6 >= maxIndices) {
				return;
			}

			const float quadWidth = (frameCoords.x1 - frameCoords.x0) * 2.0f;// / materialResource.width;
			const float quadHeight = (frameCoords.y1 - frameCoords.y0) * 2.0f;// / materialResource.height;

			const TextureCoordinates factoredCoords {
				frameCoords.x0 / materialResource.width,
				frameCoords.y0 / materialResource.height,
				frameCoords.x1 / materialResource.width,
				frameCoords.y1 / materialResource.height
			};

			PosColorVertex* vertexHead = reinterpret_cast<PosColorVertex*>(transientBuffer.transientVertexBuffer.data) + startVertex;
			vertexHead[0] = { spatial.x,  spatial.y, spatial.z, factoredCoords.x0, factoredCoords.y0 };
			vertexHead[1] = { spatial.x,  spatial.y + quadHeight, spatial.z, factoredCoords.x0, factoredCoords.y1 };
			vertexHead[2] = { spatial.x + quadWidth,  spatial.y + quadHeight, spatial.z, factoredCoords.x1, factoredCoords.y1 };
			vertexHead[3] = {spatial.x + quadWidth,  spatial.y, spatial.z, factoredCoords.x1, factoredCoords.y0 };

			uint16_t* indexHead = reinterpret_cast<uint16_t*>(transientBuffer.transientIndexBuffer.data) + startIndex;
			// 0, 1, 3,
			indexHead[0] = startVertex;
			indexHead[1] = startVertex + 1;
			indexHead[2] = startVertex + 3;
			// 3, 1, 2
			indexHead[3] = startVertex + 3;
			indexHead[4] = startVertex + 1;
			indexHead[5] = startVertex + 2;

			++transientBuffer.quadCount;
		});

		for (const auto& [materialEntity, transientBufferData] : perMaterialTransientBuffers) {
			if (materialEntity == entt::null || !registry.all_of<MaterialResource>(materialEntity)) {
				continue;
			}

			const MaterialResource& materialResource = registry.get<MaterialResource>(materialEntity);
			if (!registry.all_of<BGFXProgram>(materialResource.shaderProgramResource) ||
				!registry.all_of<BGFXTexture>(materialResource.textureImageResource)) {
				return;
			}

			const auto& bgfxProgram = registry.get<BGFXProgram>(materialResource.shaderProgramResource);
			const auto& bgfxTexture = registry.get<BGFXTexture>(materialResource.textureImageResource);

			float mtx[16];
			bx::mtxIdentity(mtx);
			bgfx::setTransform(mtx);

			bgfx::setVertexBuffer(0, &transientBufferData.transientVertexBuffer, 0, transientBufferData.quadCount * 4);
			bgfx::setIndexBuffer(&transientBufferData.transientIndexBuffer, 0, transientBufferData.quadCount * 6);
			bgfx::setTexture(0, mTexColourUniformHandle,  bgfxTexture.textureHandle);

			const auto& alphaColour = materialResource.alphaColour.value_or({ 255.0f, 255.0f, 255.0f, 255.0f });
			bgfx::setUniform(mAlphaColourUniformHandle, &alphaColour);

			// Set render states.
			bgfx::setState(0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				//| BGFX_STATE_WRITE_Z
				//| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
				| BGFX_STATE_BLEND_EQUATION_ADD
				//| BGFX_STATE_MSAA
			);

			// Submit primitive for rendering to view 0.
			bgfx::submit(0, bgfxProgram.programHandle);
		}

		auto drawCallbackView = registry.view<BGFXDrawCallback>();
		drawCallbackView.each([&registry](const BGFXDrawCallback& drawCallback) {
			drawCallback.drawCallback(registry);
		});

		bgfx::frame();
	}
	
} // Core
