export module Gfx.FontRenderSystem;

import Core.ResourceHandle;
import Core.Scheduler;
import entt;

export namespace Gfx {

	class FontRenderSystem {
	public:

		FontRenderSystem(entt::registry&, Core::Scheduler&);
		~FontRenderSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

		Core::ResourceHandlePtr mFontShaderProgram;

	};

} // namespace Gfx
