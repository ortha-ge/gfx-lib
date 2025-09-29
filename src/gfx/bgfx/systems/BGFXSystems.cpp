module;

#include <cstdio>
#include <optional>
#include <string>
#include <utility>

#include <bx/bx.h>
#include <bgfx/bgfx.h>

module Gfx.BGFX.Systems;

import Core.Log;
import Core.NativeWindowHandles;
import Core.Window;
import Gfx.ShaderPlatformInfo;
import Gfx.BGFX.BGFXContext;
import Gfx.BGFX.BGFXFrameBuffer;

namespace Gfx::BGFX::BGFXSystemsInternal {

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

} //

namespace Gfx::BGFX {

	BGFXSystems::BGFXSystems(entt::registry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		connectEnTTSignals(registry);

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	BGFXSystems::~BGFXSystems() {
		mScheduler.unschedule(std::move(mTickHandle));

		if (mBGFXContext != entt::null) {
			bgfx::shutdown();
		}
	}

	void BGFXSystems::tickSystem(entt::registry& registry) {

	}

	void BGFXSystems::tryCreateBGFXContext(entt::registry& registry) {

	}

	void BGFXSystems::onNativeWindowHandlesCreated(entt::registry& registry, const entt::entity entity) {
		using namespace BGFXSystemsInternal;

		if (mBGFXContext != entt::null) {
			return;
		}

		auto&& [nativeWindowHandles, window] = registry.get<Core::NativeWindowHandles, Core::Window>(entity);

		if (!nativeWindowHandles.windowHandle) {
			return;
		}

		mBGFXContext = registry.create();
		registry.emplace<BGFXContext>(mBGFXContext, entity);
		auto& callbacks = registry.emplace<BGFXCallbacks>(mBGFXContext, registry, mBGFXContext);

		bgfx::Init initData;
		initData.platformData.nwh = nativeWindowHandles.windowHandle;
		initData.platformData.ndt = nativeWindowHandles.displayHandle;
		initData.resolution.width = window.width;
		initData.resolution.height = window.height;
		initData.callback = &callbacks;

		if (!bgfx::init(initData)) {
			Core::logEntry(registry, "Failed to initialize BGFX.");
			registry.destroy(mBGFXContext);
			return;
		}

		Core::logEntry(registry, "BGFX initialized.");

		registry.emplace<BGFXFrameBuffer>(entity, BGFXFrameBuffer{
			.handle = BGFX_INVALID_HANDLE
		});

		registry.emplace<ShaderPlatformInfo>(mBGFXContext, getShaderPlatformInfo());
	}

	void BGFXSystems::connectEnTTSignals(entt::registry& registry) {
		Core::logEntry(registry, "BGFX waiting for window handle...");
		registry.on_construct<Core::NativeWindowHandles>().connect<&BGFXSystems::onNativeWindowHandlesCreated>(this);
	}
} // namespace Gfx::BGFX
