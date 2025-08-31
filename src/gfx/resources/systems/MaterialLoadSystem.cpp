module;

#include <entt/entt.hpp>

module Gfx.MaterialLoadSystem;

import Core.EnTTRegistry;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ImageDescriptor;
import Core.JsonTypeLoaderAdapter;
import Core.TypeLoader;
import Gfx.MaterialDescriptor;
import Gfx.MaterialResource;
import Gfx.ShaderProgramDescriptor;

namespace Gfx {

    MaterialLoadSystem::MaterialLoadSystem() {
    }

    void MaterialLoadSystem::tickSystem(Core::EnTTRegistry& registry) {
        entt::registry& _registry(registry);

        auto createProgramResourceView = _registry.view<const MaterialDescriptor>(entt::exclude<MaterialResource>);
        createProgramResourceView.each([&registry = _registry](entt::entity entity, const MaterialDescriptor& materialDescriptor) {
            const auto programResource = registry.create();
            registry.emplace<Core::FileDescriptor>(programResource, materialDescriptor.shaderProgramFilePath);
            registry.emplace<Core::TypeLoader>(programResource, std::make_unique<Core::JsonTypeLoaderAdapter<Gfx::ShaderProgramDescriptor>>());
            registry.emplace<Core::FileLoadRequest>(programResource);

            const auto imageResource = registry.create();
            registry.emplace<Core::FileDescriptor>(imageResource, materialDescriptor.textureImageFilePath);
            registry.emplace<Core::ImageDescriptor>(imageResource);
            registry.emplace<Core::FileLoadRequest>(imageResource);

            registry.emplace<MaterialResource>(entity,
                programResource,
                imageResource,
                materialDescriptor.spriteFrames,
                materialDescriptor.alphaColour,
                materialDescriptor.width,
                materialDescriptor.height
            );
        });
    }

} // Gfx
