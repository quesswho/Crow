#pragma once
#include "Common.h"
#include "graphics/Window.h"
#include "Event/Input.h"
#include "Event/Events.h"
#include "graphics/LayerManager.h"

namespace Crow {

	class Application
	{
	public:
		explicit Application();
		virtual ~Application();

		static std::unique_ptr<Input> m_Input;
		static void OnEvent(Event& appEvent);
		void Run();
		static bool s_Closed;

		static void Shutdown();

		//Wrapper functions for Crow::LayerManager because m_LayerManager is private
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
	private:
		virtual void OnUpdate();
	private:
		std::unique_ptr<Window> m_Window;
		static std::unique_ptr<LayerManager> s_LayerManager;
	};
}

