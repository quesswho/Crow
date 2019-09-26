#pragma once
#include "Crow/Graphics/Window.h"
#include "Crow/Common.h"
#include <GLFW/glfw3.h>

namespace Crow {
	namespace Platform {

		class GLFWAPIWindow : public Window // API so it is easier to differentiate between GLFWwindow included by glfw3.h
		{
		public:
			GLFWAPIWindow(WindowProperties properties);
			virtual ~GLFWAPIWindow() override;

			static Window* CreateGLFWWindow(WindowProperties props) { return new GLFWAPIWindow(props); }

			virtual void Update() const override;

			virtual void CreateDeviceContex() override;

			virtual void SetTitle(const char* title) override;

			virtual void* GetHandle() const override { return m_Window; }
		private:
			void SetCallbacks() const;
			void Init();

			WindowProperties m_Properties;
			GLFWwindow* m_Window;
		};
	}
}

