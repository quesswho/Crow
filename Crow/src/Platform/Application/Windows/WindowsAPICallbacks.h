#pragma once
#include "Crow/Common.h"
#ifdef CR_PLATFORM_WINDOWS
	#include <Windows.h>
#else
	struct HWND {};
#endif

namespace Crow {
	namespace Platform {
		class WindowsAPICallbacks
		{
		public:
			static void error_callback(int error, const char* description);
			static void close_callback();
			static void window_size_callback(int width, int height);
			static void key_callback(int flags, int key, uint msg);
			static void mouse_button_callback(HWND hWnd, uint button);
			static void cursor_position_callback(int xpos, int ypos);
		private:
			static uint DecodeWindowsKey(uint key); // Turns windows key codes to crow key code (glfw key codes)
		};

	}
}