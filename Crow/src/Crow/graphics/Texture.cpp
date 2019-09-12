#include "Texture.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	Texture* Texture::Create(const char* path, const TextureProperties& props)
	{
		return Platform::PlatformAPI::CreateTexture(path, props);
	}
}