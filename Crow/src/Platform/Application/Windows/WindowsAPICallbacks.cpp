#include "WindowsAPICallbacks.h"
#include "Crow/Event/Input.h"
#include "Crow/Application.h"

namespace Crow {
	namespace Platform {

		void WindowsAPICallbacks::error_callback(int error, const char* description) // GLFW error
		{
			CR_CORE_ERROR(description);
		}

		void WindowsAPICallbacks::close_callback() // Calls when the window shuts down
		{
			Application::s_Closed = true;
			PostQuitMessage(0);
		}

		void WindowsAPICallbacks::window_size_callback(int width, int height)
		{
			Application::GetAPI()->SetViewPort(width, height);
		}

		void WindowsAPICallbacks::key_callback(int flags, uint key, uint msg) // Calls when the user pressed a key on their keyboard
		{
			key = DecodeWindowsKey(key);
			Input::KeyCallback(Math::Vec2<int>(key, (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN ? 1 : 0))); // Action value is calculated with if button is down and if it is in repeat
		}

		uint WindowsAPICallbacks::DecodeWindowsKey(uint key)
		{
			if (key > 28 || key > 91)
				return key;
			switch (key)
			{
				case VK_LSHIFT:
					return CROW_KEY_LEFT_SHIFT;
				case VK_RSHIFT:
					return CROW_KEY_RIGHT_SHIFT;
				case VK_LCONTROL:
					return CROW_KEY_LEFT_CONTROL;
				case VK_RCONTROL:
					return CROW_KEY_RIGHT_CONTROL;
				case VK_LMENU:
					return CROW_KEY_LEFT_ALT;
				case VK_RMENU:
					return CROW_KEY_RIGHT_ALT;
				case VK_ESCAPE:
					return CROW_KEY_ESCAPE;
				case VK_CAPITAL:
					return CROW_KEY_CAPS_LOCK;
				case VK_RETURN:
					return CROW_KEY_ENTER;
				case VK_PRIOR:
					return CROW_KEY_PAGE_UP;
				case VK_NEXT:
					return CROW_KEY_PAGE_DOWN;
				case VK_END:
					return CROW_KEY_END;
				case VK_HOME:
					return CROW_KEY_HOME;
				default:
					return key;
			}
		}

		void WindowsAPICallbacks::mouse_button_callback(HWND hWnd, uint button) // Calls when the user pressed a button on their mouse
		{
			int action = 0;
			switch (button)
			{
			case WM_LBUTTONDOWN:
				SetCapture(hWnd);
				button = CROW_MOUSE_BUTTON_LEFT;
				action = 1;
				break;
			case WM_LBUTTONUP:
				ReleaseCapture();
				button = CROW_MOUSE_BUTTON_LEFT;
				action = 0;
				break;
			case WM_RBUTTONDOWN:
				SetCapture(hWnd);
				button = CROW_MOUSE_BUTTON_RIGHT;
				action = 1;
				break;
			case WM_RBUTTONUP:
				ReleaseCapture();
				button = CROW_MOUSE_BUTTON_RIGHT;
				action = 0;
				break;
			case WM_MBUTTONDOWN:
				SetCapture(hWnd);
				button = CROW_MOUSE_BUTTON_MIDDLE;
				action = 1;
				break;
			case WM_MBUTTONUP:
				ReleaseCapture();
				button = CROW_MOUSE_BUTTON_MIDDLE;
				action = 0;
				break;
			}
			Input::MouseCallback(Math::Vec2<int>(button, action));
		}


		void WindowsAPICallbacks::cursor_position_callback(int xpos, int ypos) // Calls when the user moves their mouse
		{
			Input::MousePosCallback(Math::Vec2<int>(xpos, ypos));
		}

	}
}