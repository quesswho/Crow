#pragma once
#include "Crow/Common.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Crow {

	struct WindowProperties // Title, width and height struct
	{
		const char* m_Title;
		const uint m_Width;
		const uint m_Height;
		WindowProperties(const char* title, const uint width, const uint height)
			: m_Title(title), m_Width(width), m_Height(height)
		{}
	};

	
	class Window
	{
	public:
		Window(WindowProperties properties);
		~Window();

		void Update() const;

		void SetTitle(const char* title);

		const std::string GetGraphicsInfo();
	private:
		void setCallbacks() const;
		void Init();

		WindowProperties m_Properties;
		GLFWwindow* m_Window;
	};
}

