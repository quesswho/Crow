#pragma once
#include "Common.h"
#include "Window.h"
#include "Event/Input.h"
#include "Event/Events.h"

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
	private:
		virtual void Update();
	private:
		std::unique_ptr<Window> m_Window;
	};
}

