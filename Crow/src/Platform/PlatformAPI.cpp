#include "PlatformAPI.h"

#include "Crow/Application.h"

namespace Crow {
	namespace Platform {

		ApplicationAPI PlatformAPI::s_ApplicationAPI;

		void PlatformAPI::ApplicationAPIInit(ApplicationAPI appApi)
		{
			switch(appApi)
			{
				case ApplicationAPI::GLFW:
					s_ApplicationAPI = ApplicationAPI::GLFW;
					GLFWInit();
					MathInit();
					break;
				case ApplicationAPI::WINDOWS:
					CR_WINDOWSERROR();
					s_ApplicationAPI = ApplicationAPI::WINDOWS;
					WindowsInit();
					MathInit();
					break;
			}
		}

		void PlatformAPI::GLFWInit()
		{
			CreateWindowAPI = &GLFWAPIWindow::CreateGLFWWindow;
		}

		void PlatformAPI::WindowsInit()
		{
			CreateWindowAPI = &WindowsAPIWindow::CreateWindowsWindow;
		}

		void PlatformAPI::MathInit()
		{
#ifdef CROW_OGL
			Math::MATH_COORDINATE::s_MathCoordinateType = Math::MATH_COORDINATE::MATH_COORDINATE_RIGHTHAND;
#elif defined(CROW_DX11)
			Math::MATH_COORDINATE::s_MathCoordinateType = Math::MATH_COORDINATE::MATH_COORDINATE_LEFTHAND;
#endif
		}


		bool PlatformAPI::CheckWindowsError()
		{
			if (s_ApplicationAPI != ApplicationAPI::WINDOWS)
			{
				CR_CORE_WARNING("Cannot use a non Windows application for DirectX! Using OpenGL instead.");
				return false;
			}
			return true;
		}

	}
}