module;

#include <list>

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>

export module Gfx.ShaderProgramLoadSystem;

import Core.EnTTRegistry;
import Gfx.ShaderProgramDescriptor;

export namespace Gfx {

	class ShaderProgramLoadSystem {
	public:
		ShaderProgramLoadSystem();

		void tickSystem(entt::registry&);

	private:

		void _tryCreateShaderProgramResource(entt::registry&, entt::entity, const ShaderProgramDescriptor&);
		void _tryCleanupTrackedShaderProgramResources(entt::registry&);

		struct TrackedShaderProgramResources {
			entt::entity entity{entt::null};
			entt::entity vsShaderEntity{entt::null};
			entt::entity fsShaderEntity{entt::null};
		};

		std::list<TrackedShaderProgramResources> mTrackedShaderPrograms{};
	};

} // namespace Gfx
