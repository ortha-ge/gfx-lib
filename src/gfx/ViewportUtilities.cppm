module;

#include <optional>

export module Ortha.Gfx.ViewportUtilities;

import Ortha.Gfx.Viewport;
import entt;
import glm;

export namespace Ortha::Gfx {

	struct ViewportScreenRect {
		glm::ivec2 bottomLeft{};
		glm::ivec2 topRight{};
	};

	std::optional<glm::ivec2> getViewportRenderTargetSize(entt::registry&, const Viewport&);
	std::optional<glm::ivec2> getViewportScreenSize(entt::registry&, const Viewport&);
	glm::ivec2 getViewportScreenSize(const ViewportScreenRect&);
	std::optional<ViewportScreenRect> getViewportScreenRect(entt::registry&, const Viewport&);

} // namespace Ortha::Gfx
