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

			if (GetSystemMetrics(SM_CXSCREEN) < m_Properties.m_Width || GetSystemMetrics(SM_CYSCREEN) < m_Properties.m_Height)
			{
				CR_CORE_WARNING("Application resolution is more than the resolution of the screen!");
			}

			m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, wc.lpszClassName, L"Crow Engine", WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION,
				0, 0, m_Properties.m_Width, m_Properties.m_Height, NULL, NULL, m_hInstance, NULL);

			// Get the position for the window
			int posX, posY;

			RECT rect;
			GetClientRect(m_Hwnd, &rect);

			int x1 = GetSystemMetrics(SM_CXFRAME);
			int x2 = GetSystemMetrics(SM_CXEDGE);
			int x3 = GetSystemMetrics(SM_CXSMSIZE);
			int y1 = GetSystemMetrics(SM_CYFRAME);
			int y3 = GetSystemMetrics(SM_CYEDGE);
			int y2 = GetSystemMetrics(SM_CYCAPTION);
			int y4 = GetSystemMetrics(SM_CYSMSIZE);
			posX = (GetSystemMetrics(SM_CXSCREEN) - m_Properties.m_Width) / 2 - (GetSystemMetrics(SM_CXSMSIZE) - GetSystemMetrics(SM_CXEDGE) - GetSystemMetrics(SM_CXFRAME)) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - m_Properties.m_Height) / 2 - GetSystemMetrics(SM_CYCAPTION) - (GetSystemMetrics(SM_CYSMSIZE) + GetSystemMetrics(SM_CYEDGE)) / 2  + GetSystemMetrics(SM_CYFRAME);

			SetWindowPos(m_Hwnd, HWND_TOP, posX, posY, m_Properties.m_Width + GetSystemMetrics(SM_CXSMSIZE) - GetSystemMetrics(SM_CXEDGE) - GetSystemMetrics(SM_CXFRAME), m_Properties.m_Height + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSMSIZE) - GetSystemMetrics(SM_CYEDGE) - GetSystemMetrics(SM_CYFRAME), 0);



			if (!m_Hwnd)
			{
				CR_CORE_FATAL("Could not create window!");
				return;
			}

			RawMouseInit();
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
				Input::s_Focused = true;
				break;
			case WM_KILLFOCUS:
				Input::s_Focused = false;
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
				break;
			case WM_INPUT:
			{
				UINT dwSize;
				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
					sizeof(RAWINPUTHEADER));
				LPBYTE lpb = new BYTE[dwSize];
				if (lpb == NULL)
				{
					return 0;
				}

				GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
					lpb, &dwSize, sizeof(RAWINPUTHEADER));

				RAWINPUT* raw = (RAWINPUT*)lpb;

				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					Input::MouseChange(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
				break;
			}
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

#ifdef CROW_OGL 
			SwapBuffers(m_Hdc);
#endif
		}

		void WindowsAPIWindow::SetTitle(const char* title)
		{
			m_Properties.m_Title = title;
			SetWindowTextA(m_Hwnd, m_Properties.m_Title);
		}

		void WindowsAPIWindow::SetCursorPosition(Math::TVec2<int> pos)
		{
			POINT pt;
			pt.x = pos.x;
			pt.y = pos.y;
			ClientToScreen(m_Hwnd, &pt);
			SetCursorPos(pt.x, pt.y);
			Input::SetVMousePosition(Math::TVec2<double>(pos.x, pos.y));
		}

		void WindowsAPIWindow::SetCursorVisibility(bool visibility)
		{
			ShowCursor(visibility);
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

		void WindowsAPIWindow::RawMouseInit() const
		{
			RAWINPUTDEVICE rid[1];
			rid[0].usUsagePage = ((USHORT)0x01);
			rid[0].usUsage = ((USHORT)0x02);
			rid[0].dwFlags = RIDEV_INPUTSINK;
			rid[0].hwndTarget = m_Hwnd;
			RegisterRawInputDevices(rid, 1, sizeof(rid[0]));
		}
	}
}