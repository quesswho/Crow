#include "Application.h"
#include "Log.h"
#include <stdio.h>

namespace Crow {

	bool Application::m_Closed;
	std::unique_ptr<LayerManager> Application::m_LayerManager;

	Application::Application()
	{
		Log::Init();
		CR_CORE_INFO("Initialized Log!");

		m_Window = std::make_unique<Window>(WindowProperties("Crow Engine", 800, 600));
		m_Closed = false;
		CR_CORE_INFO("Window has been created!");

		Input::Init();
		CR_CORE_INFO("Input class has been Initialized!");

		m_LayerManager = std::make_unique<LayerManager>();
	}


	Application::~Application()
	{
		m_Closed = true;
		m_Window->~Window();
	}

	void Application::Shutdown()
	{
		m_Closed = true;
	}

	void Application::Run()
	{
		while (!m_Closed)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_TRIANGLES);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glEnd();
			OnUpdate();
		}
		Application::~Application();
	}

	void Application::OnUpdate()
	{
		m_Window->Update();

		for (auto it = m_LayerManager->begin(); it != m_LayerManager->end(); it++)
			(*it)->OnUpdate();
	}

	void Application::OnEvent(Event& appEvent)
	{
		//Process the events
		for (auto it = m_LayerManager->end(); it != m_LayerManager->begin();)
			(*--it)->OnEvent(appEvent);

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerManager->PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerManager->PopLayer(layer);
	}
}
