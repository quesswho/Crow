#pragma once
#include "Common.h"
#include "graphics/Window.h"
#include "Event/Input.h"
#include "Event/Events.h"
#include "graphics/LayerManager.h"

namespace Crow {

	class CROW_API Application
	{
	public:
		explicit Application();
		virtual ~Application();

		static std::unique_ptr<Input> m_Input;
		static void OnEvent(Event& appEvent);
		void Run();
		static bool m_Closed;

		static void Shutdown();

		//Wrapper functions for Crow::LayerManager because m_LayerManager is private
		inline void PushLayer(Layer* layer);
		inline void PopLayer(Layer* layer);
	private:
		virtual void OnUpdate();
	private:
		std::unique_ptr<Window> m_Window;
		static std::unique_ptr<LayerManager> m_LayerManager;
	};
}

