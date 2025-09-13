module;

#include <entt/entt.hpp>

module Gfx.SpriteLoadSystem;

import Core.ProcessError;
import Gfx.Sprite;

namespace Gfx {

	SpriteLoadSystem::SpriteLoadSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	SpriteLoadSystem::~SpriteLoadSystem() {

	}

	void SpriteLoadSystem::tickSystem(entt::registry& registry) {
		registry.view<SpriteDescriptor>(entt::exclude<Sprite, Core::ProcessError>)
			.each([&registry](const entt::entity entity, const SpriteDescriptor& spriteDescriptor) {
				tryCreateSprite(registry, entity, spriteDescriptor);
			});
	}

	void SpriteLoadSystem::tryCreateSprite(
		entt::registry& registry, const entt::entity entity, const SpriteDescriptor& spriteDescriptor) {

		if (spriteDescriptor.frames.empty()) {
			Core::addProcessError(registry, entity, "Sprite frames are empty.");
			return;
		}

		registry.emplace<Sprite>(entity, spriteDescriptor);
	}

} // namespace Gfx
