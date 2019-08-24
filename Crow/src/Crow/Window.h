#pragma once
#include "Common.h"
#include <GLFW/glfw3.h>

namespace Crow {

	struct CROW_API WindowProperties // Title, width and height struct
	{
		const char* m_Title;
		const uint m_Width;
		const uint m_Height;
		WindowProperties(const char* title, const uint width, const uint height)
			: m_Title(title), m_Width(width), m_Height(height)
		{}
	};

	class CROW_API Window
	{
	public:
		Window(WindowProperties properties);
		~Window();

		void Update();
	private:
		void setCallbacks();
		void Init();
		WindowProperties m_Properties;
		GLFWwindow* m_Window;
	};
}

