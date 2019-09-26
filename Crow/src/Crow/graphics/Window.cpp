#include "Window.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	Window* Window::CreateWindowAPI(WindowProperties props)
	{
		return Platform::PlatformAPI::CreateWindowAPI(props);
	}
}
