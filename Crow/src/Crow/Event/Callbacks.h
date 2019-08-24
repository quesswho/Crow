#pragma once
#include "Crow/Common.h"
#include <GLFW/glfw3.h>

namespace Crow {

	class Callbacks
	{
	public:
		void static error_callback(int error, const char* description);
		void static close_callback(GLFWwindow* window);
		void static window_size_callback(GLFWwindow* window, int width, int height);
		void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void static mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	};

}
