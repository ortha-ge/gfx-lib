module;

#include <cstddef>

#include <entt/entity/entity.hpp>

export module Gfx.RenderCommand;

export namespace Gfx {

	struct RenderCommand {
		entt::entity viewportEntity{ entt::null }; // Could be added onto a viewport commands list instead of holding viewport entity
		entt::entity shaderProgram{ entt::null };
		entt::entity vertexBuffer{ entt::null };
		entt::entity indexBuffer{ entt::null };
		size_t vertexCount{ 0zu };
		size_t indexCount{ 0zu };
		uint16_t renderPass{ 0u };

		// TEMP
		entt::entity texture{ entt::null };

	};

} // namespace Gfx
