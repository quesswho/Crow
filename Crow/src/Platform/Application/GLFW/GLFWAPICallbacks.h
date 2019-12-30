#pragma once

#include <GLFW/glfw3.h>

namespace Crow {
	namespace Platform {
		class GLFWCallbacks
		{
		public:
			static void error_callback(int error, const char* description);
			static void close_callback(GLFWwindow* window);
			static void window_size_callback(GLFWwindow* window, int width, int height);
			static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
			static void window_focus_callback(GLFWwindow* window, int focused);
		};

	}
}