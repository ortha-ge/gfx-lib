module;

#include <vector>

#include <bimg/decode.h>
#include <bx/error.h>
#include <entt/entt.hpp>
#include <tracy/Tracy.hpp>

module Gfx.ImageLoadSystem;

import Core.FileLoadRequest;
import Core.Log;
import Core.ProcessError;
import Core.RawDataResource;
import Gfx.Image;
import Gfx.ImageDescriptor;

namespace Gfx {

	ImageLoadSystem::ImageLoadSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler }
		, mAllocator{ std::make_unique<bx::DefaultAllocator>() } {

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	ImageLoadSystem::~ImageLoadSystem() { mScheduler.unschedule(std::move(mTickHandle)); }


	void ImageLoadSystem::tickSystem(entt::registry& registry) {
		ZoneScopedN("ImageLoadSystem::tickSystem");
		using namespace Core;

		registry.view<const ImageDescriptor, const RawDataResource>(entt::exclude<FileLoadRequest, ProcessError, Image>)
			.each([&registry](
					  entt::entity entity, const ImageDescriptor& imageDescriptor,
					  const RawDataResource& rawDataResource) {
				bx::DefaultAllocator allocator{};
				bx::Error error{};
				bimg::ImageContainer* image = bimg::imageParse(
					&allocator, rawDataResource.rawData.data(), rawDataResource.size,
					bimg::TextureFormat::Enum::RGBA32F, &error);
				if (image) {
					auto* readHead = static_cast<uint8_t*>(image->m_data);
					registry.emplace<Image>(
						entity, std::vector<uint8_t>(readHead, &readHead[image->m_size]),
						static_cast<uint32_t>(image->m_format), image->m_size, image->m_offset, image->m_width,
						image->m_height, image->m_depth, image->m_numLayers, image->m_numMips, image->m_hasAlpha,
						image->m_cubeMap, image->m_ktx, image->m_ktxLE, image->m_pvr3, image->m_srgb);
					bimg::imageFree(image);
					image = nullptr;
				} else {
					addProcessError(registry, entity, "ImageLoadSystem parsing error: {}", error.getMessage().getCPtr());
				}
			});
	}

} // namespace Gfx
