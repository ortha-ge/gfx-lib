
export module Gfx.MaterialLoadSystem;

import Core.EnTTRegistry;

export namespace Gfx {

    class MaterialLoadSystem {
    public:

        MaterialLoadSystem();

        void tickSystem(Core::EnTTRegistry& registry);

    };

} // Gfx
