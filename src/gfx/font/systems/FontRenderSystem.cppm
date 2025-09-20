module;

#include <entt/fwd.hpp>

export module Gfx.FontRenderSystem;

import Core.EnTTRegistry;
import Core.ResourceHandle;
import Core.Scheduler;

export namespace Gfx {

	class FontRenderSystem {
	public:

		FontRenderSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~FontRenderSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

		Core::ResourceHandlePtr mFontShaderProgram;

	};

} // namespace Gfx
