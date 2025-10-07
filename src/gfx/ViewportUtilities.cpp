module;

#include <cstdint>
#include <optional>

module Ortha.Gfx.ViewportUtilities;

import Ortha.Core.Window;
import Ortha.Gfx.RenderTexture;

namespace Ortha::Gfx {

	std::optional<glm::ivec2> getViewportRenderTargetSize(entt::registry& registry, const Viewport& viewport) {
		using namespace Ortha::Core;
		entt::entity renderTarget = viewport.renderTarget;
		if (renderTarget == entt::null) {
			//TODO: Select main window
			auto windowView = registry.view<Window>();
			if (windowView.empty()) {
				return std::nullopt;
			}

			renderTarget = windowView.front();
		}

		if (registry.all_of<Window>(renderTarget)) {
			const auto& window{ registry.get<Window>(renderTarget) };
			return glm::ivec2{ window.width, window.height };
		}

		if (registry.all_of<RenderTexture>(renderTarget)) {
			const auto& renderTexture{ registry.get<RenderTexture>(renderTarget) };
			return glm::ivec2{ renderTexture.width, renderTexture.height };
		}

		return std::nullopt;
	}

	std::optional<glm::ivec2> getViewportScreenSize(entt::registry& registry, const Viewport& viewport) {
		auto size = getViewportRenderTargetSize(registry, viewport);
		if (!size) {
			return std::nullopt;
		}

		return glm::ivec2{
			static_cast<int32_t>((viewport.offset.x + viewport.dimensions.x) * static_cast<float>(size->x)),
			static_cast<int32_t>((viewport.offset.y + viewport.dimensions.y) * static_cast<float>(size->y))
		};
	}

	glm::ivec2 getViewportScreenSize(const ViewportScreenRect& screenRect) {
		return glm::ivec2 {
			screenRect.topRight.x - screenRect.bottomLeft.x,
			screenRect.bottomLeft.y - screenRect.topRight.y
		};
	}

	std::optional<ViewportScreenRect> getViewportScreenRect(entt::registry& registry, const Viewport& viewport) {
		auto size = getViewportRenderTargetSize(registry, viewport);
		if (!size) {
			return std::nullopt;
		}

		return ViewportScreenRect {
			.bottomLeft = glm::ivec2{ viewport.offset.x * size->x, viewport.dimensions.y * size->y },
			.topRight = glm::ivec2{ viewport.dimensions.x * size->x, viewport.offset.y * size->y }
		};
	}

} // namespace Ortha::Gfx
