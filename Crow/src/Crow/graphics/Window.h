#pragma once
#include "Crow/Common.h"

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

		static Window* CreateWindowAPI(WindowProperties properties);
		virtual ~Window() = default;
		
		virtual void Update() const = 0;

		virtual void CreateDeviceContex() = 0;

		virtual void SetTitle(const char* title) = 0;

		virtual void* GetHandle() const = 0;
	};
}

