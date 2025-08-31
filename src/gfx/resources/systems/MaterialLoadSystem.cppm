module;

#include <entt/entity/entity.hpp>
#include <list>

export module Gfx.MaterialLoadSystem;

import Core.EnTTRegistry;

export namespace Gfx {

	class MaterialLoadSystem {
	public:
		MaterialLoadSystem();

		void tickSystem(Core::EnTTRegistry &registry);

	private:
		struct TrackedMaterialResources {
			entt::entity entity{entt::null};
			entt::entity shaderProgramEntity{entt::null};
			entt::entity textureEntity{entt::null};
		};

		std::list<TrackedMaterialResources> mTrackedMaterials{};
	};

} // namespace Gfx
