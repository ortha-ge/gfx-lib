module;

#include <memory>
#include <utility>

module Ortha.Gfx.MaterialLoadSystem;

import Ortha.Core.FileDescriptor;
import Ortha.Core.FileLoadRequest;
import Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.ResourceHandle;
import Ortha.Core.ResourceLoadRequest;
import Ortha.Core.TypeLoader;
import Ortha.Gfx.ImageDescriptor;
import Ortha.Gfx.Material;
import Ortha.Gfx.MaterialDescriptor;
import Ortha.Gfx.Reflection.ShaderProgramDescriptor;

namespace Ortha::Gfx {

	MaterialLoadSystem::MaterialLoadSystem(entt::registry& registry, Core::Scheduler& scheduler)
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

} // namespace Ortha::Gfx
