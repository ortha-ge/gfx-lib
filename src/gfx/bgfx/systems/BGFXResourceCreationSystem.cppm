module;

#include <entt/fwd.hpp>

export module Gfx.BGFX.BGFXResourceCreationSystem;

import Core.EnTTRegistry;
import Core.RawDataResource;
import Core.Scheduler;
import Gfx.Image;
import Gfx.IndexBuffer;
import Gfx.RenderTexture;
import Gfx.ShaderProgram;
import Gfx.VertexBuffer;

export namespace Gfx::BGFX {

	class BGFXResourceCreationSystem {
	public:

		BGFXResourceCreationSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~BGFXResourceCreationSystem();

		void tickSystem(entt::registry&);

		static void _tryCreateShader(entt::registry&, entt::entity, const Core::RawDataResource&);
		static void _tryCreateShaderProgram(entt::registry&, entt::entity, const ShaderProgram&);
		static void _tryCreateVertexLayout(entt::registry&, entt::entity, const ShaderProgram&);
		static void _tryCreateUniforms(entt::registry&, entt::entity, const ShaderProgram&);
		static void _tryCreateTexture(entt::registry&, entt::entity, const Image&);
		static void _tryCreateFrameBuffer(entt::registry&, entt::entity, const RenderTexture&);
		static void _tryCreateTransientVertexBuffer(entt::registry&, entt::entity&, const VertexBuffer&);
		static void _tryCreateTransientIndexBuffer(entt::registry&, entt::entity&, const IndexBuffer&);

	private:

		void connectEnTTSignals(entt::registry&);

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Gfx::BGFX
