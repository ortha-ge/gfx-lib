export module Ortha.Gfx.MaterialLoadSystem;

import Ortha.Core.Scheduler;
import Ortha.Gfx.MaterialDescriptor;
import entt;

export namespace Ortha::Gfx {

	class MaterialLoadSystem {
	public:
		MaterialLoadSystem(entt::registry& registry, Core::Scheduler& scheduler);
		~MaterialLoadSystem();

		void tickSystem(entt::registry&);

	private:
		void _tryCreateMaterialResource(entt::registry&, entt::entity, const MaterialDescriptor&);

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;
	};

} // namespace Ortha::Gfx
