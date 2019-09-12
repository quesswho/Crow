#pragma once

#include "Object2D.h"

namespace Crow {

	class Renderer2D
	{
	private:
		std::vector<Object2D*> m_Renderables;
	public:

		Renderer2D();

		~Renderer2D();

		void Begin() {}
		void Submit(Object2D* renderable);
		void Submit(std::vector<Object2D*> renderables);
		void End() {}
		void Flush();
	};
}