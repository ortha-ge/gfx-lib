module;

#include <functional>
#include <unordered_map>
#include <vector>

#include <entt/entity/entity.hpp>

export module Gfx.RenderCandidates;

import Core.TypeId;

export namespace Gfx {

	struct RenderCandidateBucket {
		using EntityList = std::vector<entt::entity>;
		std::function<void(EntityList& entityList, entt::entity)> callback;
		EntityList entityList;
	};

	struct RenderCandidates {
		std::unordered_map<Core::TypeId, RenderCandidateBucket, Core::TypeIdHasher> candidateBuckets;
	};

} // namespace Gfx
