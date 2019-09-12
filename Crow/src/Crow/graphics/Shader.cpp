#include "Shader.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	Shader* Shader::CreateFromPath(const char* name, const char* path)
	{
		return Platform::PlatformAPI::CreateShaderFromPath(name, path);
	}

	Shader* Shader::CreateFromSource(const char* name, std::string& source)
	{
		return Platform::PlatformAPI::CreateShaderFromSource(name, source);
	}
}