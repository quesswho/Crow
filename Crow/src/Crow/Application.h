#pragma once
#include "Common.h"
#include "graphics/Window.h"
#include "Event/Input.h"
#include "Event/Events.h"
#include "graphics/LayerManager.h"
#include "Timer.h"
#include "Platform/PlatformAPI.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <freetype/freetype.h>

namespace Crow {

	class Application
	{
	public:
		Application()
			: Application(WindowProperties("Crow Engine", 720, 720), Platform::GraphicAPI::OPENGL, Platform::ApplicationAPI::GLFW)
		{}

		Application(WindowProperties winprop, Platform::GraphicAPI graphicApi, Platform::ApplicationAPI appApi);
		
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

		static WindowProperties s_WindowProperties;

		static void SetCursorPosition(Math::TVec2<int> pos);
		static void SetCursorVisibility(bool visbility);

		static FT_Library s_FreeTypeLibrary;

		static void BindPostEffect(PostEffect* postfx) { s_PostEffect = postfx; }
	private:
		virtual void OnUpdate(float elapsed);
	private:
		static Window* s_Window;
		static std::unique_ptr<LayerManager> s_LayerManager;
		static std::unique_ptr<Timer> m_Timer;
		static AbstractRenderAPI* s_RenderAPI;
		static PostEffect* s_PostEffect;
	};
}

