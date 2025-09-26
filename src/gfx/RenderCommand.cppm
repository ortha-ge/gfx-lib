module;

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

#include <entt/entity/entity.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

export module Gfx.RenderCommand;

import Core.Any;

export namespace Gfx {

	struct RenderCommand {
		entt::entity viewportEntity{ entt::null }; // Could be added onto a viewport commands list instead of holding viewport entity
		entt::entity shaderProgram{ entt::null };
		entt::entity vertexBuffer{ entt::null };
		entt::entity indexBuffer{ entt::null };
		size_t vertexOffset{ 0zu };
		size_t vertexCount{ 0zu };
		size_t indexOffset{ 0zu };
		size_t indexCount{ 0zu };
		uint16_t renderPass{ 0u };
		std::optional<uint64_t> bgfxState{};
		std::unordered_map<std::string, Core::Any> uniformData;
		std::optional<glm::ivec4> scissorRect{};
		size_t sortDepth{ 0zu};
		glm::mat4 modelMatrix{ 1.0f };
	};

} // namespace Gfx
