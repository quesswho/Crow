#pragma once
#include "Crow/Event/Events.h"
#include "Crow/Macros.h"

namespace Crow {

	class Layer {
	public:
		Layer();
		~Layer();

		virtual void OnRender() {}
		virtual void OnEvent(Event& e) {}
	};
}

