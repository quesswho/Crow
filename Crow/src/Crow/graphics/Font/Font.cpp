#include "Font.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	Font* Font::Create(const char* path)
	{
		return Platform::PlatformAPI::CreateTTFont(path);
	}

	Font* Font::Create(const char* path, Shader* shader)
	{
		return Platform::PlatformAPI::CreateTTSFont(path, shader);
	}
}