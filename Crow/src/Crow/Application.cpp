#include "Application.h"
#include "graphics/Renderer/RenderAPI.h"
#include "Log.h"
#include "graphics/ShaderManager.h"
#include <stdio.h>
#include <string>

namespace Crow {

	bool Application::s_Closed;
	std::unique_ptr<LayerManager> Application::s_LayerManager;
	std::unique_ptr<Timer> Application::m_Timer;
	int Application::m_FramesPerSecond;


	Application::Application()
	{
		Log::Init();
		CR_CORE_INFO("Initialized Log!");

		m_Window = std::make_unique<Window>(WindowProperties("Crow Engine", 800, 600));
		s_Closed = false;

		Input::Init();
		CR_CORE_INFO("Input class has been Initialized!");

		s_LayerManager = std::make_unique<LayerManager>();

		m_Timer = std::make_unique<Timer>();
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Shutdown()
	{
		s_Closed = true;
	}

	void Application::Run()
	{
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
				m_Window->SetTitle(std::string("Crow Engine : ").append(std::to_string(m_FramesPerSecond)).append(" FPS").c_str());
				frames = 0;
				elapsed = 0;
			}
			else {
				frames++;
				continue;
			}
		}
	}

	void Application::OnUpdate(double elapsed)
	{
		RenderAPI::Clear();
		for (auto it = s_LayerManager->begin(); it != s_LayerManager->end(); it++)
		{
			(*it)->OnRender();
			(*it)->OnUpdate(elapsed);
		}

		m_Window->Update();
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
}
