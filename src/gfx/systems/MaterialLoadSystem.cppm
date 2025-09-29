export module Gfx.MaterialLoadSystem;

import Core.Scheduler;
import Gfx.MaterialDescriptor;
import entt;

export namespace Gfx {

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

} // namespace Gfx
