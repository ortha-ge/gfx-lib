module;

#include <list>

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>

export module Gfx.MaterialLoadSystem;

import Core.EnTTRegistry;
import Core.Scheduler;
import Gfx.MaterialDescriptor;

export namespace Gfx {

	class MaterialLoadSystem {
	public:
		MaterialLoadSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler);
		~MaterialLoadSystem();

		void tickSystem(entt::registry&);

	private:
		void _tryCreateMaterialResource(entt::registry&, entt::entity, const MaterialDescriptor&);

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;
	};

} // namespace Gfx
