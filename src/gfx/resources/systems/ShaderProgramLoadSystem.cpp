module;

#include <entt/entt.hpp>

module Gfx.ShaderProgramLoadSystem;

import Core.EnTTRegistry;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ResourceLoadRequest;
import Gfx.ShaderDescriptor;
import Gfx.ShaderProgramDescriptor;
import Gfx.ShaderProgramResource;

namespace Gfx {

	ShaderProgramLoadSystem::ShaderProgramLoadSystem() = default;

	void ShaderProgramLoadSystem::tickSystem(entt::registry& registry) {
		auto createProgramResourceView = registry.view<const ShaderProgramDescriptor>(entt::exclude<ShaderProgramResource>);
		createProgramResourceView.each([this, &registry](entt::entity entity, const ShaderProgramDescriptor &programDescriptor) {
			_tryCreateShaderProgramResource(registry, entity, programDescriptor);
		});

		_tryCleanupTrackedShaderProgramResources(registry);
	}

	void ShaderProgramLoadSystem::_tryCreateShaderProgramResource(entt::registry& registry,
		entt::entity entity, const ShaderProgramDescriptor& shaderProgramDescriptor) {

		const auto vsResource = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(vsResource,
			Core::ResourceLoadRequest::create<ShaderDescriptor>(shaderProgramDescriptor.vertexShaderFilePath)
		);

		const auto fsResource = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(fsResource,
			Core::ResourceLoadRequest::create<ShaderDescriptor>(shaderProgramDescriptor.fragmentShaderFilePath)
		);

		registry.emplace<ShaderProgramResource>(entity, vsResource, fsResource);

		mTrackedShaderPrograms.emplace_back(entity, vsResource, fsResource);
	}

	void ShaderProgramLoadSystem::_tryCleanupTrackedShaderProgramResources(entt::registry& registry) {
		for (auto it = mTrackedShaderPrograms.begin(); it != mTrackedShaderPrograms.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->entity) && registry.all_of<ShaderProgramResource>(currentIt->entity)) {
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
