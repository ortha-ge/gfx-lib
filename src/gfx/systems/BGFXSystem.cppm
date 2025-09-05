module;

#include <bgfx/bgfx.h>
#include <entt/fwd.hpp>

export module Gfx.BGFXSystem;

import Core.EnTTRegistry;
import Core.ImageResource;
import Core.RawDataResource;
import Gfx.BGFXContext;
import Gfx.ShaderProgramResource;

export namespace Gfx {

	class BGFXSystem {
	public:

		BGFXSystem(Core::EnTTRegistry&);
		~BGFXSystem();

		void initSystem(entt::registry& registry);
		void destroySystem(entt::registry& registry);
		void tickSystem(entt::registry& registry);

	private:

		void onWindowInternalCreated(entt::registry&, entt::entity);
		static void _destroyDroppedTextures(const entt::registry&, BGFXContext& context);
		void _destroyTrackedTextures(BGFXContext& context) const;

		static void _tryCreateShader(BGFXContext&, entt::registry&, entt::entity, const Core::RawDataResource&);
		static void _tryCreateShaderProgram(BGFXContext&, entt::registry&, entt::entity, const ShaderProgramResource&);
		static void _tryCreateVertexLayout(BGFXContext&, entt::registry&, entt::entity, const ShaderProgramResource&);
		static void _tryCreateUniforms(BGFXContext&, entt::registry&, entt::entity, const ShaderProgramResource&);
		static void _tryCreateTexture(BGFXContext&, entt::registry&, entt::entity, const Core::ImageResource&);

		Core::EnTTRegistry& mRegistry;

	};

} // Gfx