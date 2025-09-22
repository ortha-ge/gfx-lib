module;

#include <utility>

#include <entt/entt.hpp>

module Gfx.ImageAtlasLoadSystem;

import Core.ProcessError;
import Core.ResourceLoadRequest;
import Gfx.ImageAtlas;
import Gfx.ImageAtlasDescriptor;
import Gfx.ImageDescriptor;

namespace Gfx {

	ImageAtlasLoadSystem::ImageAtlasLoadSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	ImageAtlasLoadSystem::~ImageAtlasLoadSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void ImageAtlasLoadSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		registry.view<ImageAtlasDescriptor>(entt::exclude<ProcessError, ImageAtlas>)
			.each([&registry](const entt::entity entity, const ImageAtlasDescriptor& imageAtlasDescriptor) {
				ImageAtlas imageAtlas;
				imageAtlas.image = ResourceLoadRequest::create<ImageDescriptor>(registry, imageAtlasDescriptor.imageFilePath);
				imageAtlas.regions = imageAtlasDescriptor.regions;

				registry.emplace<ImageAtlas>(entity, std::move(imageAtlas));
			});
	}

} // namespace Gfx
