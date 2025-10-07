module;

#include <utility>

module Ortha.Gfx.ImageAtlasLoadSystem;

import Ortha.Core.ProcessError;
import Ortha.Core.ResourceLoadRequest;
import Ortha.Gfx.ImageAtlas;
import Ortha.Gfx.ImageAtlasDescriptor;
import Ortha.Gfx.ImageDescriptor;

namespace Ortha::Gfx {

	ImageAtlasLoadSystem::ImageAtlasLoadSystem(entt::registry& registry, Core::Scheduler& scheduler)
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
		using namespace Ortha::Core;
		registry.view<ImageAtlasDescriptor>(entt::exclude<ProcessError, ImageAtlas>)
			.each([&registry](const entt::entity entity, const ImageAtlasDescriptor& imageAtlasDescriptor) {
				ImageAtlas imageAtlas;
				imageAtlas.image = ResourceLoadRequest::create<ImageDescriptor>(registry, imageAtlasDescriptor.imageFilePath);
				imageAtlas.regions = imageAtlasDescriptor.regions;

				registry.emplace<ImageAtlas>(entity, std::move(imageAtlas));
			});
	}

} // namespace Ortha::Gfx
