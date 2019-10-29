#include "GLFWAPIWindow.h"

namespace Crow {
	namespace Platform {

		GLFWAPIWindow::GLFWAPIWindow(WindowProperties properties)
			: m_Properties(properties)
		{
			Init();
		}


		GLFWAPIWindow::~GLFWAPIWindow()
		{
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}

		void GLFWAPIWindow::Init()
		{
			if (!glfwInit()) CR_CORE_ERROR("Failed to initialize GLFW!");


			m_Window = glfwCreateWindow(m_Properties.m_Width, m_Properties.m_Height, m_Properties.m_Title, NULL, NULL);
			if (!m_Window) {
				CR_CORE_ERROR("Failed to create Window!");
				glfwTerminate();
				return;
			}

			int posX, posY;

			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			posX = (mode->width - m_Properties.m_Width) / 2;
			posY = (mode->height - m_Properties.m_Height) / 2;

			if (posX < 0 || posY < 0)
			{
				posX = 0;
				posY = 0;
				CR_CORE_WARNING("Application resolution is more than the resolution of the screen!");
			}

			glfwSetWindowPos(m_Window, posX, posY);

			CreateDeviceContex();

			SetCallbacks();
		}

		void GLFWAPIWindow::Update() const
		{
			glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}

		void GLFWAPIWindow::CreateDeviceContex()
		{
			glfwMakeContextCurrent(m_Window);
		}

		void GLFWAPIWindow::SetCallbacks() const
		{
			glfwSetErrorCallback(&Callbacks::error_callback);
			glfwSetKeyCallback(m_Window, &Callbacks::key_callback);
			glfwSetWindowSizeCallback(m_Window, &Callbacks::window_size_callback);
			glfwSetMouseButtonCallback(m_Window, &Callbacks::mouse_button_callback);
			glfwSetCursorPosCallback(m_Window, &Callbacks::cursor_position_callback);
		}

		void GLFWAPIWindow::SetTitle(const char* title)
		{
			m_Properties.m_Title = title;
			glfwSetWindowTitle(m_Window, title);
		}
	}
}