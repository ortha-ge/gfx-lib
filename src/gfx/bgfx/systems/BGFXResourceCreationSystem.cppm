export module Ortha.Gfx.BGFX.BGFXResourceCreationSystem;

import Ortha.Core.RawDataResource;
import Ortha.Core.Scheduler;
import Ortha.Gfx.Image;
import Ortha.Gfx.IndexBuffer;
import Ortha.Gfx.RenderTexture;
import Ortha.Gfx.ShaderProgram;
import Ortha.Gfx.VertexBuffer;
import entt;

export namespace Ortha::Gfx::BGFX {

	class BGFXResourceCreationSystem {
	public:

		BGFXResourceCreationSystem(entt::registry&, Core::Scheduler&);
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

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Ortha::Gfx::BGFX
