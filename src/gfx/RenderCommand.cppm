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
import Gfx.RenderState;

export namespace Gfx {

	struct RenderCommand {
		entt::entity shaderProgram{ entt::null };
		std::unordered_map<std::string, Core::Any> uniformData;

		// Viewport
		entt::entity viewportEntity{ entt::null };
		std::optional<glm::ivec4> scissorRect{};

		RenderState renderState{};
		glm::mat4 modelMatrix{ 1.0f };
		uint32_t sortDepth{ 0u };

		// TODO: RenderPass is difficult to expose since it has to be per-viewport
		uint16_t renderPass{ 0u };

		// Vertex Buffer
		entt::entity vertexBuffer{ entt::null };
		size_t vertexOffset{ 0zu };
		size_t vertexCount{ 0zu };

		// Index Buffer
		entt::entity indexBuffer{ entt::null };
		size_t indexOffset{ 0zu };
		size_t indexCount{ 0zu };

	};

} // namespace Gfx
