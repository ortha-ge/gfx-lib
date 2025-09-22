module;

#include <entt/entt.hpp>

module Gfx.ShaderProgramLoadSystem;

import Core.EnTTRegistry;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ProcessError;
import Core.ResourceHandle;
import Core.ResourceLoadRequest;
import Gfx.ShaderDescriptor;
import Gfx.ShaderPlatformInfo;
import Gfx.ShaderProgram;
import Gfx.ShaderProgramDescriptor;

namespace Gfx::ShaderProgramLoadSystemInternal {
	std::string getMappedShaderFilePath(const std::string& shaderFolderPath, std::string shaderFilePath) {
		if (auto forwardSlashPos = shaderFilePath.find_last_of('/'); forwardSlashPos != std::string::npos) {
			shaderFilePath.replace(forwardSlashPos, 1, "/" + shaderFolderPath + "/");
		} else if (auto backSlashPos = shaderFilePath.find_last_of('\\'); backSlashPos != std::string::npos) {
			shaderFilePath.replace(backSlashPos, 1, "\\" + shaderFolderPath + "\\");
		}

		return shaderFilePath + ".bin";
	}
}

namespace Gfx {

	ShaderProgramLoadSystem::ShaderProgramLoadSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	ShaderProgramLoadSystem::~ShaderProgramLoadSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void ShaderProgramLoadSystem::tickSystem(entt::registry& registry) {
		using namespace Core;

		const auto& platformView = registry.view<ShaderPlatformInfo>();
		if (platformView.empty()) {
			return;
		}

		const auto& shaderPlatformInfo{ registry.get<ShaderPlatformInfo>(platformView.front()) };

		auto createProgramResourceView = registry.view<const ShaderProgramDescriptor>(entt::exclude<ProcessError, ShaderProgram>);
		createProgramResourceView.each(
			[this, &shaderPlatformInfo, &registry](entt::entity entity, const ShaderProgramDescriptor& programDescriptor) {
				_tryCreateShaderProgramResource(registry, entity, shaderPlatformInfo, programDescriptor);
			});

		_tryCleanupTrackedShaderProgramResources(registry);
	}

	void ShaderProgramLoadSystem::_tryCreateShaderProgramResource(
		entt::registry& registry, entt::entity entity, const ShaderPlatformInfo& shaderPlatformInfo, const ShaderProgramDescriptor& shaderProgramDescriptor) {
		using namespace Core;
		using namespace ShaderProgramLoadSystemInternal;

		if (!shaderPlatformInfo.shadersFolderName) {
			return;
		}

		if (shaderProgramDescriptor.vertexShaderFilePath.empty()) {
			addProcessError(registry, entity, "Empty vertex shader file path in shader program creation.");
			return;
		}

		if (shaderProgramDescriptor.fragmentShaderFilePath.empty()) {
			addProcessError(registry, entity, "Empty fragment shader file path in shader program creation.");
			return;
		}

		if (shaderProgramDescriptor.vertexLayout.attributes.empty()) {
			addProcessError(registry, entity, "Empty vertex layout attributes in shader program creation.");
			return;
		}

		std::string vertexShaderFilePath = getMappedShaderFilePath(*shaderPlatformInfo.shadersFolderName, shaderProgramDescriptor.vertexShaderFilePath);
		std::string fragmentShaderFilePath = getMappedShaderFilePath(*shaderPlatformInfo.shadersFolderName, shaderProgramDescriptor.fragmentShaderFilePath);

		auto vsResource =
			Core::ResourceLoadRequest::create<ShaderDescriptor>(registry, vertexShaderFilePath);

		auto fsResource = Core::ResourceLoadRequest::create<ShaderDescriptor>(
			registry, fragmentShaderFilePath);

		registry.emplace<ShaderProgram>(
			entity, vsResource, fsResource, shaderProgramDescriptor.uniforms, shaderProgramDescriptor.vertexLayout);

		//mTrackedShaderPrograms.emplace_back(entity, vsResource, fsResource);
	}

	void ShaderProgramLoadSystem::_tryCleanupTrackedShaderProgramResources(entt::registry& registry) {
		for (auto it = mTrackedShaderPrograms.begin(); it != mTrackedShaderPrograms.end();) {
			auto currentIt = it++;
			if (registry.valid(currentIt->entity) && registry.all_of<ShaderProgram>(currentIt->entity)) {
				continue;
			}

			if (registry.valid(currentIt->vsShaderEntity)) {
				registry.destroy(currentIt->vsShaderEntity);
			}

			if (registry.valid(currentIt->fsShaderEntity)) {
				registry.destroy(currentIt->fsShaderEntity);
			}

			mTrackedShaderPrograms.erase(currentIt);
		}
	}

} // namespace Gfx
