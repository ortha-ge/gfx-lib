module;

#include <entt/entt.hpp>

module Gfx.MaterialLoadSystem;

import Core.EnTTRegistry;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ImageDescriptor;
import Core.JsonTypeLoaderAdapter;
import Core.ResourceLoadRequest;
import Core.TypeLoader;
import Gfx.MaterialDescriptor;
import Gfx.MaterialResource;
import Gfx.ShaderProgramDescriptor;
import Gfx.Reflection.ShaderProgramDescriptor;

namespace Gfx {

	MaterialLoadSystem::MaterialLoadSystem() = default;

	void MaterialLoadSystem::tickSystem(entt::registry& registry) {
		registry.view<const MaterialDescriptor>(entt::exclude<MaterialResource>)
				.each([this, &registry](entt::entity entity, const MaterialDescriptor& materialDescriptor) {
					_tryCreateMaterialResource(registry, entity, materialDescriptor);
				});

		_tryCleanupTrackedMaterialResources(registry);
	}

	void MaterialLoadSystem::_tryCreateMaterialResource(entt::registry& registry, entt::entity entity,
														const MaterialDescriptor& materialDescriptor) {

		const auto programResource = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(
				programResource,
				Core::ResourceLoadRequest::create<Core::TypeLoader>(
						materialDescriptor.shaderProgramFilePath,
						std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::ShaderProgramDescriptor>>()));

		const auto imageResource = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(
				imageResource,
				Core::ResourceLoadRequest::create<Core::ImageDescriptor>(materialDescriptor.textureImageFilePath));

		registry.emplace<MaterialResource>(entity, programResource, imageResource, materialDescriptor.spriteFrames,
										   materialDescriptor.alphaColour, materialDescriptor.width,
										   materialDescriptor.height);

		mTrackedMaterials.emplace_back(entity, programResource, imageResource);
	}

	void MaterialLoadSystem::_tryCleanupTrackedMaterialResources(entt::registry& registry) {
		for (auto it = mTrackedMaterials.begin(); it != mTrackedMaterials.end();) {
			auto currentIt = it++;

			if (registry.valid(currentIt->entity) && registry.all_of<MaterialResource>(currentIt->entity)) {
				continue;
			}

			if (registry.valid(currentIt->textureEntity)) {
				registry.destroy(currentIt->textureEntity);
			}

			if (registry.valid(currentIt->shaderProgramEntity)) {
				registry.destroy(currentIt->shaderProgramEntity);
			}

			mTrackedMaterials.erase(currentIt);
		}
	}

} // namespace Gfx
