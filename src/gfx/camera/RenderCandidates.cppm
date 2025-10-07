module;

#include <functional>
#include <unordered_map>
#include <vector>

export module Ortha.Gfx.RenderCandidates;

import Ortha.RTTI.TypeId;
import entt;

export namespace Ortha::Gfx {

	struct RenderCandidateBucket {
		using EntityList = std::vector<entt::entity>;
		std::function<void(EntityList& entityList, entt::entity)> callback;
		EntityList entityList;
	};

	struct RenderCandidates {
		std::unordered_map<Ortha::RTTI::TypeId, RenderCandidateBucket, Ortha::RTTI::TypeIdHasher> candidateBuckets;
	};

} // namespace Ortha::Gfx
