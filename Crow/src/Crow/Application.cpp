#include "Application.h"

#include "graphics/Renderer/RenderAPI.h"
#include "graphics/ShaderManager.h"
#include "Platform/PlatformAPI.h"
#include "Log.h"

#include <stdio.h>
#include <string>

namespace Crow {

	bool Application::s_Closed;
	std::unique_ptr<LayerManager> Application::s_LayerManager;
	std::unique_ptr<Timer> Application::m_Timer;
	int Application::m_FramesPerSecond;
	AbstractRenderAPI* Application::s_RenderAPI;
	Window* Application::s_Window;
	WindowProperties Application::s_WindowProperties;
	FT_Library Application::s_FreeTypeLibrary;

	PostEffect* Application::s_PostEffect;

	Application::Application(WindowProperties winProp, Platform::GraphicAPI graphicApi, Platform::ApplicationAPI appApi)
	{
		Log::Init();

		Platform::PlatformAPI::ApplicationAPIInit(appApi);
		Platform::PlatformAPI::GraphicAPIInit(graphicApi);

		s_WindowProperties = winProp;
		s_Window = Platform::PlatformAPI::CreateWindowAPI(s_WindowProperties);
		s_RenderAPI = Platform::PlatformAPI::CreateRenderAPI();
		s_Closed = false; // Application::s_Closed


		if (!s_RenderAPI->InitAPI(s_WindowProperties, s_Window->GetHandle()))
		{
			CR_CORE_FATAL("Failed to Initialize Graphics API: {}", s_RenderAPI->GetAPIName());
		}

		CR_CORE_INFO(s_RenderAPI->GetGraphicsInfo());
		CR_CORE_INFO("Using Graphics API: {}", s_RenderAPI->GetAPIName());

		Input::Init();

		s_LayerManager = std::make_unique<LayerManager>();
		m_Timer = std::make_unique<Timer>();


		if (FT_Init_FreeType(&s_FreeTypeLibrary))
		{
			CR_CORE_ERROR("Failed to initialize FreeType");
		}

		CR_CORE_INFO("Application has initialized successfully!");
	}

	Application::~Application()
	{
		delete s_RenderAPI;
		delete s_Window;
		FT_Done_FreeType(s_FreeTypeLibrary);
		Shutdown();
	}

	void Application::Shutdown()
	{
		s_Closed = true;
	}

	void Application::Run()
	{
		s_RenderAPI->EndInit();

		int frames = 0;
		double elapsed = 0;
		double tick = 0;
		while (!s_Closed)
		{
			m_Timer->Start();
			////////////
			OnUpdate(m_Timer->GetLastElapsed());
			///////////
			m_Timer->End();
			elapsed += m_Timer->GetElapsedTimeInSeconds();
			tick += m_Timer->GetElapsedTimeInSeconds();
			if (tick > 0.1) // If it has been 1/10 second
			{
				m_FramesPerSecond = (int)((double)frames * tick * 100.0);
				frames = 0;
				tick = 0;
			}
			else
				frames++;

			if (elapsed > 1.0)
			{
				s_Window->SetTitle(std::string(s_WindowProperties.m_Title).append(" : ").append(std::to_string(m_FramesPerSecond)).append(" FPS").c_str());
				elapsed = 0;
			}
		}
	}

	void Application::OnUpdate(float elapsed)
	{
		for (auto it = s_LayerManager->begin(); it != s_LayerManager->end(); it++)
		{
			if (s_PostEffect) s_PostEffect->Bind();
			s_RenderAPI->Begin();
			(*it)->OnRender();
			(*it)->OnUpdate(elapsed);
			if (s_PostEffect) s_PostEffect->Draw();
		}
		s_RenderAPI->End();
		s_Window->Update();
	}

	void Application::OnEvent(Event& appEvent)
	{
		//Process the events
		for (auto it = s_LayerManager->end(); it != s_LayerManager->begin();)
			(*--it)->OnEvent(appEvent);

	}

	void Application::PushLayer(Layer* layer)
	{
		s_LayerManager->PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		s_LayerManager->PopLayer(layer);
	}

	void Application::SetCursorPosition(Math::TVec2<int> pos)
	{
		s_Window->SetCursorPosition(pos);
	}

	void Application::SetCursorVisibility(bool visbility)
	{
		s_Window->SetCursorVisibility(visbility);
	}

	AbstractRenderAPI* Application::GetAPI()
	{ 
		return s_RenderAPI;
	}
}
