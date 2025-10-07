export module Ortha.Gfx.FontRenderSystem;

import Ortha.Core.ResourceHandle;
import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Gfx {

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

} // namespace Ortha::Gfx
