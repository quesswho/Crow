#include "Application.h"
#include "Log.h"
#include <stdio.h>

namespace Crow {

	bool Application::m_Closed;

	Application::Application()
	{
		Log::Init();
		CR_CORE_INFO("Initialized Log!");

		m_Window = std::make_unique<Window>(WindowProperties("Crow Engine", 800, 600));
		m_Closed = false;
		CR_CORE_INFO("Window has been created!");

		Input::Init();
		CR_CORE_INFO("Input class has been Initialized!");
	}


	Application::~Application()
	{
		m_Closed = true;
		m_Window->~Window();
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
			Update();
		}
		Application::~Application();
	}

	void Application::Update()
	{
		m_Window->Update();
	}

	void Application::OnEvent(Event& appEvent)
	{
		//Process the events
		auto[key, action] = appEvent.getKeyValue();
		switch (appEvent.m_Type) 
		{
			case KEY:
				CR_CORE_INFO("Keyboard: {}", key);
				if (key == CROW_KEY_ESCAPE && action == CROW_KEY_PRESS)
					m_Closed = true;
				break;
			case MOUSE:
				CR_CORE_INFO("Mouse Button: {}", key);
				break;
			case MOUSEPOS: // key = x, action = y
			//	CR_CORE_INFO("Mouse Pos: {}, {}", key, action);
				break;
		}

	}
}
