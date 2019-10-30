#include "Crow/Application.h" // Fix redifinition of APIENTRY

#include "GLFWAPICallbacks.h"

#include "Crow/Event/Input.h"

namespace Crow {
	namespace Platform {

		void Callbacks::error_callback(int error, const char* description) // GLFW error
		{
			CR_CORE_ERROR(description);
		}

		void Callbacks::close_callback(GLFWwindow* window) // Calls when the user pressed the red x
		{
			Application::s_Closed = true;
		}

		void Callbacks::window_size_callback(GLFWwindow* window, int width, int height)
		{
			Application::GetAPI()->SetViewPort(width, height);
		}

		void Callbacks::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) // Calls when the user pressed a key on their keyboard
		{
			Input::KeyCallback(Math::Vec2<int>(key, action));
		}

		void Callbacks::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) // Calls when the user pressed a button on their mouse
		{
			Input::MouseCallback(Math::Vec2<int>(button, action));
		}

		void Callbacks::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) // Calls when the user moves their mouse
		{
			Input::MousePosCallback(Math::Vec2<int>((int)xpos, (int)ypos));
		}

	}
}