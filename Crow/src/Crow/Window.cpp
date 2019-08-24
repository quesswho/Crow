#include "Window.h"
#include "Event/Callbacks.h"
namespace Crow {

	Window::Window(WindowProperties properties)
		: m_Properties(properties)
	{
		Init();
	}


	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::Init()
	{
		CR_CORE_ASSERT(glfwInit(), "Failed to initialize glfw!");

		m_Window = glfwCreateWindow(m_Properties.m_Width, m_Properties.m_Height, m_Properties.m_Title, NULL, NULL);
		CR_CORE_ASSERT(m_Window, "Failed to create Window");

		glfwMakeContextCurrent(m_Window);

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
