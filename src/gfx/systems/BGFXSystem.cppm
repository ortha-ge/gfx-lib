module;

#include <bgfx/bgfx.h>
#include <entt/fwd.hpp>

export module Gfx.BGFXSystem;

import Core.EnTTRegistry;
import Core.RawDataResource;
import Core.Scheduler;
import Gfx.BGFXContext;
import Gfx.Image;
import Gfx.ShaderProgram;

export namespace Gfx {

	class BGFXSystem {
	public:
		BGFXSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~BGFXSystem();

		void initSystem(entt::registry& registry);
		void destroySystem(entt::registry& registry);
		void tickSystem(entt::registry& registry);

	private:
		void onWindowInternalCreated(entt::registry&, entt::entity);
		static void _destroyDroppedTextures(const entt::registry&, BGFXContext& context);
		void _destroyTrackedTextures(BGFXContext& context) const;

		static void _tryCreateShader(BGFXContext&, entt::registry&, entt::entity, const Core::RawDataResource&);
		static void _tryCreateShaderProgram(BGFXContext&, entt::registry&, entt::entity, const ShaderProgram&);
		static void _tryCreateVertexLayout(BGFXContext&, entt::registry&, entt::entity, const ShaderProgram&);
		static void _tryCreateUniforms(BGFXContext&, entt::registry&, entt::entity, const ShaderProgram&);
		static void _tryCreateTexture(BGFXContext&, entt::registry&, entt::entity, const Image&);

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;

		Core::TaskHandle mTickHandle{};
	};

} // namespace Gfx
