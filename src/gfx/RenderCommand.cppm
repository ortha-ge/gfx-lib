module;

#include <cstdint>
#include <string>
#include <unordered_map>

#include <entt/entity/entity.hpp>

export module Gfx.RenderCommand;

import Core.Any;

export namespace Gfx {

	struct RenderCommand {
		entt::entity viewportEntity{ entt::null }; // Could be added onto a viewport commands list instead of holding viewport entity
		entt::entity shaderProgram{ entt::null };
		entt::entity vertexBuffer{ entt::null };
		entt::entity indexBuffer{ entt::null };
		size_t vertexCount{ 0zu };
		size_t indexCount{ 0zu };
		uint16_t renderPass{ 0u };
		std::unordered_map<std::string, Core::Any> uniformData;

	};

} // namespace Gfx
