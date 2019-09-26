#include "Application.h"
#include "graphics/Renderer/RenderAPI.h"
#include "Log.h"
#include "graphics/ShaderManager.h"
#include <stdio.h>
#include <string>
#include "Platform/PlatformAPI.h"
namespace Crow {

	bool Application::s_Closed;
	std::unique_ptr<LayerManager> Application::s_LayerManager;
	std::unique_ptr<Timer> Application::m_Timer;
	int Application::m_FramesPerSecond;
	AbstractRenderAPI* Application::s_RenderAPI;
	Window* Application::s_Window;

	Application::Application(const char* title, Platform::GraphicAPI graphicApi, Platform::ApplicationAPI appApi)
		: m_ShortTitle(title)
	{
		Log::Init();

		Platform::PlatformAPI::ApplicationAPIInit(appApi);
		Platform::PlatformAPI::GraphicAPIInit(graphicApi);

		WindowProperties props = WindowProperties(m_ShortTitle, 720, 720);
		s_Window = Platform::PlatformAPI::CreateWindowAPI(props);
		s_RenderAPI = Platform::PlatformAPI::CreateRenderAPI();
		s_Closed = false; // Application::s_Closed


		if (!s_RenderAPI->InitAPI(props, s_Window->GetHandle()))
		{
			CR_CORE_FATAL("Failed to Initialize Graphics API: {}", s_RenderAPI->GetAPIName());
		}

		CR_CORE_INFO(s_RenderAPI->GetGraphicsInfo());
		CR_CORE_INFO("Using Graphics API: {}", s_RenderAPI->GetAPIName());

		Input::Init();

		s_LayerManager = std::make_unique<LayerManager>();

		m_Timer = std::make_unique<Timer>();
		CR_CORE_INFO("Application has initialized successfully!");
	}

	Application::~Application()
	{
		delete s_RenderAPI;
		delete s_Window;
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
		while (!s_Closed)
		{
			m_Timer->Start();
			////////////
			OnUpdate(m_Timer->GetLastElapsed());
			///////////
			m_Timer->End();
			elapsed += m_Timer->GetElapsedTimeInSeconds();
			if (elapsed > 1.0) // If it has been 1 second
			{
				m_FramesPerSecond = frames;
				s_Window->SetTitle(std::string(m_ShortTitle).append(" : ").append(std::to_string(m_FramesPerSecond)).append(" FPS").c_str());
				frames = 0;
				elapsed = 0;
			}
			else {
				frames++;
				continue;
			}
		}
	}

	void Application::OnUpdate(float elapsed)
	{
		s_RenderAPI->Clear();
		s_RenderAPI->Begin();
		for (auto it = s_LayerManager->begin(); it != s_LayerManager->end(); it++)
		{
			(*it)->OnRender();
			(*it)->OnUpdate(elapsed);
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

	AbstractRenderAPI* Application::GetAPI()
	{ 
		return s_RenderAPI;
	}
}
