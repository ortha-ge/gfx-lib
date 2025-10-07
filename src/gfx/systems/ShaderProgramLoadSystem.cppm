module;

#include <list>

export module Ortha.Gfx.ShaderProgramLoadSystem;

import Ortha.Core.Scheduler;
import Ortha.Gfx.ShaderPlatformInfo;
import Ortha.Gfx.ShaderProgramDescriptor;
import entt;

export namespace Ortha::Gfx {

	class ShaderProgramLoadSystem {
	public:
		ShaderProgramLoadSystem(entt::registry& registry, Core::Scheduler& scheduler);
		~ShaderProgramLoadSystem();

		void tickSystem(entt::registry&);

	private:
		void _tryCreateShaderProgramResource(entt::registry&, entt::entity, const ShaderPlatformInfo&, const ShaderProgramDescriptor&);
		void _tryCleanupTrackedShaderProgramResources(entt::registry&);

		struct TrackedShaderProgramResources {
			entt::entity entity{ entt::null };
			entt::entity vsShaderEntity{ entt::null };
			entt::entity fsShaderEntity{ entt::null };
		};

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

		std::list<TrackedShaderProgramResources> mTrackedShaderPrograms{};
	};

} // namespace Ortha::Gfx
