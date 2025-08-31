module;

#include <list>

#include <entt/entity/entity.hpp>

export module Gfx.ShaderProgramLoadSystem;

import Core.EnTTRegistry;

export namespace Gfx {

	class ShaderProgramLoadSystem {
	public:
		ShaderProgramLoadSystem();

		void tickSystem(Core::EnTTRegistry &);

	private:
		struct TrackedShaderProgramResources {
			entt::entity entity{entt::null};
			entt::entity vsShaderEntity{entt::null};
			entt::entity fsShaderEntity{entt::null};
		};

		std::list<TrackedShaderProgramResources> mTrackedShaderPrograms{};
	};

} // namespace Gfx
