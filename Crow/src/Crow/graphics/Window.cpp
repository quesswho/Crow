#include "Window.h"


#include "Crow/Event/Callbacks.h"


namespace Crow {

	Window::Window(WindowProperties properties)
		: m_Properties(properties)
	{
		Init();
	}


	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::Init()
	{
		if(!glfwInit()) CR_CORE_ERROR("Failed to initialize glfw!");

		m_Window = glfwCreateWindow(m_Properties.m_Width, m_Properties.m_Height, m_Properties.m_Title, NULL, NULL);
		if (!m_Window) {
			CR_CORE_ERROR("Failed to create Window!");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			CR_CORE_ERROR("Failed to initialize Glad!");
			return;
		}

		setCallbacks();
	}

	void Window::Update()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Window::setCallbacks()
	{
		glfwSetErrorCallback(&Callbacks::error_callback);
		glfwSetKeyCallback(m_Window, &Callbacks::key_callback);
		glfwSetWindowSizeCallback(m_Window, &Callbacks::window_size_callback);
		glfwSetMouseButtonCallback(m_Window, &Callbacks::mouse_button_callback);
		glfwSetCursorPosCallback(m_Window, &Callbacks::cursor_position_callback);
	}

}
