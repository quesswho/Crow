#pragma once
#include "Crow/Event/Events.h"
#include "Crow/Macros.h"

namespace Crow {

	class CROW_API Layer {
	public:
		Layer();
		~Layer();

		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}
	};
}

