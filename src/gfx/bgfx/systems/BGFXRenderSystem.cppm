module;

#include <bgfx/bgfx.h>
#include <entt/fwd.hpp>

export module Gfx.BGFX.BGFXRenderSystem;

import Core.Any;
import Core.EnTTRegistry;
import Core.Scheduler;
import Gfx.RenderCommand;
import Gfx.BGFX.BGFXFrameBuffer;
import Gfx.BGFX.BGFXTexture;
import Gfx.BGFX.BGFXUniform;

export namespace Gfx::BGFX {

	class BGFXRenderSystem {
	public:

		BGFXRenderSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~BGFXRenderSystem();

		void tickSystem(entt::registry&);
		void processRenderCommand(entt::registry&, const RenderCommand&);

		bool _tryBindVertexBuffer(entt::registry&, uint8_t stream, entt::entity, uint32_t offset, uint32_t count);
		bool _tryBindIndexBuffer(entt::registry&, entt::entity, uint32_t offset, uint32_t count);
		bool _tryBindFrameBuffer(entt::registry&, uint8_t viewId, entt::entity);
		bool _tryBindUniforms(entt::registry&, const BGFXUniforms&, const std::unordered_map<std::string, Core::Any>&);
		bool _tryBindEntityUniform(entt::registry&, const bgfx::UniformHandle&, entt::entity);
		bool _tryBindTextureUniform(entt::registry&, const bgfx::UniformHandle&, const BGFXTexture&);
		bool _tryBindFrameBufferUniform(entt::registry&, const bgfx::UniformHandle&, const BGFXFrameBuffer&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx::BGFX
