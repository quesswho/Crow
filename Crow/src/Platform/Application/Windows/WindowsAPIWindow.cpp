#include "WindowsAPIWindow.h"
#include "WindowsAPICallbacks.h"
#include "Crow/Application.h"

#define NOMINMAX
#undef NOGDI
#include <Windows.h>
#include <Windowsx.h>
#define NOGDI

namespace Crow {
	namespace Platform {

		WindowsAPIWindow::WindowsAPIWindow(WindowProperties properties)
			: m_Properties(properties)
		{
			Init();
		}


		WindowsAPIWindow::~WindowsAPIWindow()
		{
			ShowCursor(true);
			DestroyWindow(m_Hwnd);
		}

		void WindowsAPIWindow::Init()
		{
			s_WindowsWindowClass = this;
			m_hInstance = (HINSTANCE)& __ImageBase;

			WNDCLASSEX wc;
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.hInstance = m_hInstance;
			wc.lpfnWndProc = WndProc;
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			wc.hIconSm = wc.hIcon;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = L"WindowsWindowClass";

			if (!RegisterClassEx(&wc))
			{
				CR_CORE_FATAL("Could not initialize Windows Window!");
			}

			// Get the position for the window
			int posX, posY;

			posX = (GetSystemMetrics(SM_CXSCREEN) - m_Properties.m_Width) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - m_Properties.m_Height) / 2;

			if (posX < 0 || posY < 0)
			{
				posX = 0;
				posY = 0;
				CR_CORE_WARNING("Application resolution is more than the resolution of the screen!");
			}

			m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, wc.lpszClassName, L"Crow Engine", WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION,
				posX, posY, m_Properties.m_Width, m_Properties.m_Height, NULL, NULL, m_hInstance, NULL);

			if (!m_Hwnd)
			{
				CR_CORE_FATAL("Could not create window!");
				return;
			}

			SetTitle(m_Properties.m_Title);

			CreateDeviceContex();

			ShowWindow(m_Hwnd, SW_SHOW);
			SetForegroundWindow(m_Hwnd);
			SetFocus(m_Hwnd);
		}

		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			LRESULT result = NULL;
			if (s_WindowsWindowClass == nullptr)
				return DefWindowProc(hwnd, msg, wParam, lParam);

			switch (msg)
			{
			case WM_ACTIVATE:
			{
				if (!HIWORD(wParam)) // Is minimized
				{
					// active
				}
				else
				{
					// inactive
				}

				return 0;
			}
			case WM_SYSCOMMAND:
			{
				switch (wParam)
				{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
				}
				result = DefWindowProc(hwnd, msg, wParam, lParam);
			} break;
			case WM_SETFOCUS:
			case WM_KILLFOCUS:
				break;
			case WM_CLOSE:
			case WM_DESTROY:
				WindowsAPICallbacks::close_callback();
				break;
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
				WindowsAPICallbacks::key_callback((uint)lParam, (uint)wParam, msg);
				break;
			case WM_MOUSEMOVE:
				WindowsAPICallbacks::cursor_position_callback(LOWORD(lParam), HIWORD(lParam));
				break;
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
				WindowsAPICallbacks::mouse_button_callback(hwnd, msg);
				break;
			case WM_SIZE:
				//WindowsAPICallbacks::window_size_callback(LOWORD(lParam), HIWORD(lParam));
				break;
			default:
				result = DefWindowProc(hwnd, msg, wParam, lParam);
			}
			return result;
		}

		static PIXELFORMATDESCRIPTOR GetPixelFormat()
		{
			PIXELFORMATDESCRIPTOR result = {};
			result.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			result.nVersion = 1;
			result.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			result.iPixelType = PFD_TYPE_RGBA;
			result.cColorBits = 32;
			result.cDepthBits = 24;
			result.cStencilBits = 8;
			result.cAuxBuffers = 0;
			result.iLayerType = PFD_MAIN_PLANE;
			return result;
		}


		void WindowsAPIWindow::Update() const
		{
			MSG message;
			while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE) > 0)
			{
				if (message.message == WM_QUIT)
				{
					WindowsAPICallbacks::close_callback();
					return;
				}
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			CheckMouseMoved();

			SwapBuffers(m_Hdc);
		}

		void WindowsAPIWindow::CheckMouseMoved() const
		{
			POINT mouse;
			GetCursorPos(&mouse);
			ScreenToClient(m_Hwnd, &mouse);
			auto [mx, my] = Input::GetMousePosition();
			if (mx != mouse.x && my != mouse.y)
			{
				WindowsAPICallbacks::cursor_position_callback(mouse.x, mouse.y);
			}
		}

		void WindowsAPIWindow::SetTitle(const char* title)
		{
			m_Properties.m_Title = title;
			SetWindowTextA(m_Hwnd, m_Properties.m_Title);
		}

		void WindowsAPIWindow::CreateDeviceContex()
		{
			m_Hdc = GetDC(m_Hwnd);

			PIXELFORMATDESCRIPTOR pfd = GetPixelFormat();
			int pixelFormat = ChoosePixelFormat(m_Hdc, &pfd);
			if (pixelFormat)
			{
				if (!SetPixelFormat(m_Hdc, pixelFormat, &pfd))
				{
					CR_CORE_ERROR("Failed setting pixelformat!");
				}
			}
			
			HGLRC hrc = wglCreateContext(m_Hdc);
			if (hrc)
			{
				if (!wglMakeCurrent(m_Hdc, hrc))
				{
					CR_CORE_ERROR("Failed to set context!");
				}
			}
			else
			{
				CR_CORE_ERROR("Failed creating context!");
			}
		}

		void WindowsAPIWindow::Focus() const
		{
			SetCapture(m_Hwnd);
		}
	}
}