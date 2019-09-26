#include "Shader.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	Shader* Shader::CreateFromPath(const char* name, const char* path, const BufferProperties& shaderInput)
	{
		return Platform::PlatformAPI::CreateShaderFromPath(name, path, shaderInput);
	}

	Shader* Shader::CreateFromSource(const char* name, std::string& source, const BufferProperties& shaderInput)
	{
		return Platform::PlatformAPI::CreateShaderFromSource(name, source, shaderInput);
	}
}