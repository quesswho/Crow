#pragma once
#include "Common.h"
#include "graphics/Window.h"
#include "Event/Input.h"
#include "Event/Events.h"
#include "graphics/LayerManager.h"
#include "Timer.h"
#include "Platform/PlatformAPI.h"


namespace Crow {

	class Application
	{
	public:
		Application()
			: Application("Crow Engine", Platform::GraphicAPI::OPENGL, Platform::ApplicationAPI::GLFW)
		{}

		Application(const char* title, Platform::GraphicAPI graphicApi, Platform::ApplicationAPI appApi);

		virtual ~Application();

		static std::unique_ptr<Input> m_Input;
		static void OnEvent(Event& appEvent);
		void Run();
		static bool s_Closed;

		static void Shutdown();

		static AbstractRenderAPI* GetAPI();

		static inline void CreateDeviceContext() { s_Window->CreateDeviceContex(); }

		//Wrapper functions for Crow::LayerManager because m_LayerManager is private
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		static int m_FramesPerSecond;
	private:
		virtual void OnUpdate(float elapsed);
	private:
		static Window* s_Window;
		static std::unique_ptr<LayerManager> s_LayerManager;
		static std::unique_ptr<Timer> m_Timer;
		static AbstractRenderAPI* s_RenderAPI;

		const char* m_ShortTitle;
	};
}

