#include "PostEffect.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	PostEffect* PostEffect::CreateFromShader(Shader* shader)
	{
		return Platform::PlatformAPI::CreatePostEffectFromShader(shader);
	}

	PostEffect* PostEffect::CreateFromPath(const char* path)
	{
		return Platform::PlatformAPI::CreatePostEffectFromPath(path);
	}
}