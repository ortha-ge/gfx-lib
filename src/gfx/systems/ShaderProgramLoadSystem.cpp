module;

#include <entt/entt.hpp>

module Gfx.ShaderProgramLoadSystem;

import Core.EnTTRegistry;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ResourceHandle;
import Core.ResourceLoadRequest;
import Gfx.ShaderDescriptor;
import Gfx.ShaderProgram;
import Gfx.ShaderProgramDescriptor;

namespace Gfx {

	ShaderProgramLoadSystem::ShaderProgramLoadSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	ShaderProgramLoadSystem::~ShaderProgramLoadSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void ShaderProgramLoadSystem::tickSystem(entt::registry& registry) {
		auto createProgramResourceView = registry.view<const ShaderProgramDescriptor>(entt::exclude<ShaderProgram>);
		createProgramResourceView.each(
			[this, &registry](entt::entity entity, const ShaderProgramDescriptor& programDescriptor) {
				_tryCreateShaderProgramResource(registry, entity, programDescriptor);
			});

		_tryCleanupTrackedShaderProgramResources(registry);
	}

	void ShaderProgramLoadSystem::_tryCreateShaderProgramResource(
		entt::registry& registry, entt::entity entity, const ShaderProgramDescriptor& shaderProgramDescriptor) {

		auto vsResource =
			Core::ResourceLoadRequest::create<ShaderDescriptor>(registry, shaderProgramDescriptor.vertexShaderFilePath);

		auto fsResource = Core::ResourceLoadRequest::create<ShaderDescriptor>(
			registry, shaderProgramDescriptor.fragmentShaderFilePath);

		registry.emplace<ShaderProgram>(
			entity, vsResource, fsResource, shaderProgramDescriptor.uniforms, shaderProgramDescriptor.vertexLayout);

		//mTrackedShaderPrograms.emplace_back(entity, vsResource, fsResource);
	}

	void ShaderProgramLoadSystem::_tryCleanupTrackedShaderProgramResources(entt::registry& registry) {
		for (auto it = mTrackedShaderPrograms.begin(); it != mTrackedShaderPrograms.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->entity) && registry.all_of<ShaderProgram>(currentIt->entity)) {
				continue;
			}

			if (registry.valid(currentIt->vsShaderEntity)) {
				registry.destroy(currentIt->vsShaderEntity);
			}

			if (registry.valid(currentIt->fsShaderEntity)) {
				registry.destroy(currentIt->fsShaderEntity);
			}

			mTrackedShaderPrograms.erase(currentIt);
		}
	}

} // namespace Gfx
