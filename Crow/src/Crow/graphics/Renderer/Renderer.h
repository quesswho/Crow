#pragma once

#include <vector>

namespace Crow {

	template<typename T>
	class Renderer {
	private:
		std::vector<T*> m_Renderables;
	public:

		Renderer() {}

		virtual ~Renderer() 
		{
			for (T* renderable : m_Renderables)
			{
				delete renderable;
			}
		}

		virtual void Submit(T* renderable) { m_Renderables.push_back(renderable); }

		virtual void Flush()
		{
			for (const T* renderable : m_Renderables)
				Render(renderable);
		}
	private:
		virtual void Render(const T* renderable); //override this
	};
}