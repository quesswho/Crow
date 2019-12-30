#pragma once

#include "Crow/Graphics/Window.h"
#include "Crow/Common.h"

#ifdef CR_PLATFORM_WINDOWS 
#include "WindowsIncludes.h" 
#endif

namespace Crow {
	namespace Platform {

		class WindowsAPIWindow : public Window
		{
		public:
			WindowsAPIWindow(WindowProperties properties);
			virtual ~WindowsAPIWindow() override;

			static Window* CreateWindowsWindow(WindowProperties props) { return new WindowsAPIWindow(props); }

			virtual void Update() const override;

			virtual void SetTitle(const char* title) override;
			virtual void SetCursorPosition(Math::TVec2<int> pos) override;
			virtual void SetCursorVisibility(bool visibility) override;
			virtual void CreateDeviceContex() override;

			void Focus() const;

			virtual void* GetHandle() const override { return m_Hwnd; }
		private:
			void CheckMouseMoved() const;

			void Init();

			WindowProperties m_Properties;
#ifdef CR_PLATFORM_WINDOWS // Avoid compile errors if OS is not Windows
			HINSTANCE m_hInstance;
			HWND m_Hwnd;
			HDC m_Hdc;
#endif
		};

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static WindowsAPIWindow* s_WindowsWindowClass = 0;
	}
}

