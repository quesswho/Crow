#pragma once

#include "Renderer.h"
#include "ArrayBuffer.h"
#include "../Shader.h"


namespace Crow {

	struct Object2D {
		ArrayBuffer* m_Buffer;
		Shader* m_Shader;

		Object2D(ArrayBuffer* buffer, Shader* shader)
			: m_Buffer(buffer), m_Shader(shader)
		{}

		~Object2D()
		{
			delete m_Buffer;
			delete m_Shader;
		}

		const inline uint GetCount() const { return m_Buffer->GetCount(); }

		void Bind() const
		{
			m_Buffer->Bind();
			m_Shader->Bind();
		}

		void Unbind() const
		{
			m_Buffer->Unbind();
			m_Shader->Unbind();
		}
	};

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