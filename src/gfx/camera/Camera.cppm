module;

#include <entt/entt.hpp>

export module Gfx.Camera;

export namespace Gfx {

/*
 * Render Targets
 * ---------------
 * - Window
 * - Image (or new specialized RenderTexture class).
 *
 * - bgfx::createFrameBuffer for window and textures
 * - bgfx::setViewFrameBuffer for rendering to window (BGFX_INVALID_HANDLE is window passed to init).
 */

	struct Camera {
		entt::entity viewport { entt::null };
		entt::entity sceneRoot{ entt::null };
		float fov{ 60.0f };
	};

} // namespace Gfx
