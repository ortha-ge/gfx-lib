module;

#include <list>

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>

export module Gfx.MaterialLoadSystem;

import Core.EnTTRegistry;
import Gfx.MaterialDescriptor;

export namespace Gfx {

	class MaterialLoadSystem {
	public:
		MaterialLoadSystem();

		void tickSystem(entt::registry&);

	private:

		void _tryCreateMaterialResource(entt::registry&, entt::entity, const MaterialDescriptor&);
		void _tryCleanupTrackedMaterialResources(entt::registry&);

		struct TrackedMaterialResources {
			entt::entity entity{entt::null};
			entt::entity shaderProgramEntity{entt::null};
			entt::entity textureEntity{entt::null};
		};

		std::list<TrackedMaterialResources> mTrackedMaterials{};
	};

} // namespace Gfx
