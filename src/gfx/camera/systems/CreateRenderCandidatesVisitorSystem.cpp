module;

#include <utility>

#include <entt/entt.hpp>

module Gfx.CreateRenderCandidatesVisitorSystem;

import Core.EntityVisitor;
import Gfx.Camera;
import Gfx.RenderCandidates;

namespace Gfx {

	CreateRenderCandidatesVisitorSystem::CreateRenderCandidatesVisitorSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		}, 255);
	}

	CreateRenderCandidatesVisitorSystem::~CreateRenderCandidatesVisitorSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void CreateRenderCandidatesVisitorSystem::tickSystem(entt::registry& registry) {
		using namespace Core;

		registry.view<Camera, RenderCandidates>()
			.each([&registry](const entt::entity entity, const Camera& camera, const RenderCandidates& renderCandidates) {
				auto renderCandidatesShared = std::make_shared<RenderCandidates>(renderCandidates);
				for (auto&& [typeId, candidateBucket] : renderCandidatesShared->candidateBuckets) {
					candidateBucket.entityList.clear();
				}

				auto& entityVisitor{ registry.get_or_emplace<EntityVisitor>(entity) };
				entityVisitor.sourceEntities = { entity, camera.sceneRoot };
				entityVisitor.visitActions = {
					[&registry, renderCandidatesShared](const entt::entity entity) mutable {
						for (auto&& [typeId, candidateBucket] : renderCandidatesShared->candidateBuckets) {
							candidateBucket.callback(candidateBucket.entityList, entity);
						}
					}
				};
				entityVisitor.visitCompleteActions = {
					[&registry, entity, renderCandidatesShared] {
						registry.get_or_emplace<RenderCandidates>(entity) = std::move(*renderCandidatesShared);
					}
				};
			});
	}

} // namespace Gfx
