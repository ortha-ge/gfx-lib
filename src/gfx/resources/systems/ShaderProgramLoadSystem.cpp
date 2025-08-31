module;

#include <entt/entt.hpp>

module Gfx.ShaderProgramLoadSystem;

import Core.EnTTRegistry;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Gfx.ShaderDescriptor;
import Gfx.ShaderProgramDescriptor;
import Gfx.ShaderProgramResource;

namespace Gfx {

	ShaderProgramLoadSystem::ShaderProgramLoadSystem() = default;

	void ShaderProgramLoadSystem::tickSystem(Core::EnTTRegistry &registry) {
		entt::registry &_registry(registry);

		auto createProgramResourceView =
				_registry.view<const ShaderProgramDescriptor>(entt::exclude<ShaderProgramResource>);
		createProgramResourceView.each(
				[this, &registry = _registry](entt::entity entity, const ShaderProgramDescriptor &programDescriptor) {
					const auto vsResource = registry.create();
					registry.emplace<Core::FileDescriptor>(vsResource, programDescriptor.vertexShaderFilePath);
					registry.emplace<Core::FileLoadRequest>(vsResource);
					registry.emplace<ShaderDescriptor>(vsResource);

					const auto fsResource = registry.create();
					registry.emplace<Core::FileDescriptor>(fsResource, programDescriptor.fragmentShaderFilePath);
					registry.emplace<Core::FileLoadRequest>(fsResource);
					registry.emplace<ShaderDescriptor>(fsResource);

					registry.emplace<ShaderProgramResource>(entity, vsResource, fsResource);

					mTrackedShaderPrograms.emplace_back(entity, vsResource, fsResource);
				});

		for (auto it = mTrackedShaderPrograms.begin(); it != mTrackedShaderPrograms.end();) {
			auto currentIt = it++;
			if (_registry.valid(currentIt->entity) && _registry.all_of<ShaderProgramResource>(currentIt->entity)) {
				continue;
			}

			if (_registry.valid(currentIt->vsShaderEntity)) {
				_registry.destroy(currentIt->vsShaderEntity);
			}

			if (_registry.valid(currentIt->fsShaderEntity)) {
				_registry.destroy(currentIt->fsShaderEntity);
			}

			mTrackedShaderPrograms.erase(currentIt);
		}
	}


} // namespace Gfx
