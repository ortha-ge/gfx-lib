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
	}

	void MaterialLoadSystem::_tryCreateMaterialResource(
		entt::registry& registry, entt::entity entity, const MaterialDescriptor& materialDescriptor) {

		auto programResource = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, materialDescriptor.shaderProgramFilePath,
			std::make_shared<Core::JsonTypeLoaderAdapter<ShaderProgramDescriptor>>());

		auto imageResource =
			Core::ResourceLoadRequest::create<ImageDescriptor>(registry, materialDescriptor.textureImageFilePath);

		Material material{ programResource,
						   imageResource,
						   materialDescriptor.alphaColour};

		registry.emplace<Material>(entity, std::move(material));
	}

} // namespace Gfx
