#pragma once

// WINDOWS
#include "Application/Windows/WindowsAPIWindow.h"

// GLFW
#include "Application/GLFW/GLFWAPIWindow.h"

namespace Crow {
	namespace Platform {

		enum class ApplicationAPI {
			GLFW,
			WINDOWS
		};

		class PlatformAPI {
		public:
			static ApplicationAPI s_ApplicationAPI;

			static void ApplicationAPIInit(ApplicationAPI appApi); // Choses which ApplicationAPI will be used

			static inline Window* (*CreateWindowAPI)(WindowProperties props); // Would have wanted to call the function "CreateWindow"

		private:
			// Applications
			static void GLFWInit();
			static void WindowsInit();

			// Graphics APIs
			static void MathInit();

			static bool CheckWindowsError();
		};
	}
}