#pragma once

#include "Renderer.h"
#include "ArrayBuffer.h"
#include "../Shader.h"
#include <initializer_list>
#include "../Texture.h"

namespace Crow {

	struct Object2D {
		ArrayBuffer* m_Buffer;
		Shader* m_Shader;

		std::vector<Texture*> m_Textures;

		Object2D(ArrayBuffer* buffer, Shader* shader)
			: m_Buffer(buffer), m_Shader(shader)
		{}

		Object2D(ArrayBuffer* buffer, Shader* shader, std::initializer_list<Texture*> textures)
			: m_Buffer(buffer), m_Shader(shader), m_Textures(textures)
		{
			for (int i = 0; i < m_Textures.size(); i++)
			{
				m_Textures[i]->SetIndex(i);
			}
		}

		void AddTexture(Texture* texture)
		{
			texture->SetIndex(m_Textures.size() + 1);
			m_Textures.push_back(texture);
		}

		~Object2D()
		{
			for (int i = 0; i < m_Textures.size(); i++)
			{
				delete m_Textures[i];
			}
			delete m_Buffer;
			delete m_Shader;
		}

		const inline uint GetCount() const { return m_Buffer->GetCount(); }

		void Bind() const
		{
			for (int i = 0; i < m_Textures.size(); i++)
			{
				m_Textures[i]->Bind();
			}
			m_Buffer->Bind();
			m_Shader->Bind();
		}

		void Unbind() const
		{
			for (int i = 0; i < m_Textures.size(); i++)
			{
				m_Textures[i]->Unbind();
			}
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