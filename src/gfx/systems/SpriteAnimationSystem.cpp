module;

#include <chrono>
#include <utility>

#include <entt/entt.hpp>

module Gfx.SpriteAnimationSystem;

import Core.ResourceHandle;
import Gfx.SpriteObject;

namespace Gfx {

	SpriteAnimationSystem::SpriteAnimationSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] { tickSystem(mRegistry); });
	}

	SpriteAnimationSystem::~SpriteAnimationSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void SpriteAnimationSystem::tickSystem(entt::registry& registry) {
		const auto clockNow = std::chrono::steady_clock::now();
		registry.view<SpriteObject>().each(
			[&registry, clockNow](const entt::entity entity, SpriteObject& spriteObject) {
				tryTickSpriteAnimation(registry, entity, spriteObject, clockNow);
			});
	}

	void SpriteAnimationSystem::tryTickSpriteAnimation(
		entt::registry& registry, const entt::entity entity, SpriteObject& spriteObject,
		std::chrono::steady_clock::time_point clockNow) {

		const auto* sprite = Core::getResource<Sprite>(registry, spriteObject.spriteResource);
		if (!sprite) {
			return;
		}

		tickSpriteAnimation(*sprite, spriteObject, clockNow);
	}

	void SpriteAnimationSystem::tickSpriteAnimation(
		const Sprite& sprite, SpriteObject& spriteObject, std::chrono::steady_clock::time_point clockNow) {

		const bool canTickAnimation = (clockNow - spriteObject.lastAnimUpdateTime) > std::chrono::milliseconds(500);
		if (canTickAnimation) {
			spriteObject.currentFrame++;
			if (spriteObject.currentFrame >= sprite.frames.size()) {
				spriteObject.currentFrame = 0;
			}

			spriteObject.lastAnimUpdateTime = clockNow;
		}
	}

} // namespace Gfx
