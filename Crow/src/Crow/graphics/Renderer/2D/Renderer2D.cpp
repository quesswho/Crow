#include "Renderer2D.h"
#include "Crow/Graphics/Renderer/RenderAPI.h"
#include "Crow/Application.h"

namespace Crow {

	Renderer2D::Renderer2D() {}

	Renderer2D::~Renderer2D()
	{
		for (Object2D* renderable : m_Renderables)
		{
			delete renderable;
		}
	}

	void Renderer2D::Flush()
	{
		for (auto renderable = m_Renderables.begin(); renderable != m_Renderables.end(); ++renderable)
		{
			(*renderable)->Bind();
			Application::GetAPI()->DrawIndices((*renderable)->GetCount());
		}
		m_Renderables.clear();
	}

	void Renderer2D::Submit(Object2D* renderable) 
	{ 
		m_Renderables.push_back(renderable);
	}

	void Renderer2D::Submit(std::vector<Object2D*> other)
	{
		m_Renderables.insert(m_Renderables.end(), other.begin(), other.end());
	}
}