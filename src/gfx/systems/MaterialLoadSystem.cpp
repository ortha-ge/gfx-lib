module;

#include <entt/entt.hpp>

module Gfx.MaterialLoadSystem;

import Core.EnTTRegistry;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.JsonTypeLoaderAdapter;
import Core.ResourceHandle;
import Core.ResourceLoadRequest;
import Core.TypeLoader;
import Gfx.ImageDescriptor;
import Gfx.Material;
import Gfx.MaterialDescriptor;
import Gfx.Reflection.ShaderProgramDescriptor;

namespace Gfx {

	MaterialLoadSystem::MaterialLoadSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	MaterialLoadSystem::~MaterialLoadSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void MaterialLoadSystem::tickSystem(entt::registry& registry) {
		registry.view<const MaterialDescriptor>(entt::exclude<Material>)
			.each([this, &registry](entt::entity entity, const MaterialDescriptor& materialDescriptor) {
				_tryCreateMaterialResource(registry, entity, materialDescriptor);
			});

		_tryCleanupTrackedMaterialResources(registry);
	}

	void MaterialLoadSystem::_tryCreateMaterialResource(
		entt::registry& registry, entt::entity entity, const MaterialDescriptor& materialDescriptor) {

		auto programResource = Core::ResourceHandle::create<Core::TypeLoader>(
								 materialDescriptor.shaderProgramFilePath,
								 std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::ShaderProgramDescriptor>>());

		auto imageResource = Core::ResourceHandle::create<ImageDescriptor>(materialDescriptor.textureImageFilePath);

		auto loadRequest = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(loadRequest, programResource);

		loadRequest = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(loadRequest, imageResource);

		Material material{ programResource,
						   imageResource,
						   materialDescriptor.alphaColour,
						   materialDescriptor.width,
						   materialDescriptor.height };

		registry.emplace<Material>(entity, std::move(material));

		//mTrackedMaterials.emplace_back(entity, programResource, imageResource);
	}

	void MaterialLoadSystem::_tryCleanupTrackedMaterialResources(entt::registry& registry) {
		for (auto it = mTrackedMaterials.begin(); it != mTrackedMaterials.end();) {
			auto currentIt = it++;

			if (registry.valid(currentIt->entity) && registry.all_of<Material>(currentIt->entity)) {
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
