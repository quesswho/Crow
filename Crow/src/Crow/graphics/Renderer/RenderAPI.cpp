#include "RenderAPI.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	AbstractRenderAPI* AbstractRenderAPI::Create()
	{
		return Platform::PlatformAPI::CreateRenderAPI();
	}
}