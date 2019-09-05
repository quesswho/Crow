#include "Application.h"
#include "graphics/Renderer/RenderAPI.h"
#include "Log.h"
#include "graphics/ShaderManager.h"
#include <stdio.h>

namespace Crow {

	bool Application::s_Closed;
	std::unique_ptr<LayerManager> Application::s_LayerManager;

	Application::Application()
	{
		Log::Init();
		CR_CORE_INFO("Initialized Log!");

		m_Window = std::make_unique<Window>(WindowProperties("Crow Engine", 800, 600));
		s_Closed = false;
		CR_CORE_INFO("Window has been created!");

		Input::Init();
		CR_CORE_INFO("Input class has been Initialized!");

		s_LayerManager = std::make_unique<LayerManager>();
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
		while (!s_Closed)
		{
			OnUpdate();
		}
	}

	void Application::OnUpdate()
	{
		RenderAPI::Clear();
		for (auto it = s_LayerManager->begin(); it != s_LayerManager->end(); it++)
			(*it)->OnRender();

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
