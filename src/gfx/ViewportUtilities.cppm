module;

#include <optional>

export module Gfx.ViewportUtilities;

import Gfx.Viewport;
import entt;
import glm;

export namespace Gfx {

	struct ViewportScreenRect {
		glm::ivec2 bottomLeft{};
		glm::ivec2 topRight{};
	};

	std::optional<glm::ivec2> getViewportRenderTargetSize(entt::registry&, const Viewport&);
	std::optional<glm::ivec2> getViewportScreenSize(entt::registry&, const Viewport&);
	glm::ivec2 getViewportScreenSize(const ViewportScreenRect&);
	std::optional<ViewportScreenRect> getViewportScreenRect(entt::registry&, const Viewport&);

} // namespace Gfx
