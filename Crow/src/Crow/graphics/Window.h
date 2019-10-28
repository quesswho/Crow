#pragma once
#include "Crow/Common.h"

namespace Crow {

	struct WindowProperties // Title, width and height struct
	{
		const char* m_Title;
		uint m_Width;
		uint m_Height;
		WindowProperties(char* title, const uint width, const uint height)
			: m_Title(title), m_Width(width), m_Height(height)
		{}

		WindowProperties()
			: m_Title(""), m_Width(1), m_Height(1)
		{}

		WindowProperties& operator=(const WindowProperties& other)
		{
			this->m_Title = other.m_Title;
			this->m_Width = other.m_Width;
			this->m_Height = other.m_Height;
			return *this;
		}
	};
	
	class Window
	{
	public:

		static Window* CreateWindowAPI(WindowProperties properties);
		virtual ~Window() = default;
		
		virtual void Update() const = 0;

		virtual void CreateDeviceContex() = 0;

		virtual void SetTitle(const char* title) = 0;

		virtual void* GetHandle() const = 0;
	};
}

