#pragma once

#include "ArrayBuffer.h"
#include "../Shader.h"
#include <initializer_list>
#include "../Texture.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Crow {

	struct Object2D {
		ArrayBuffer* m_Buffer;
		Shader* m_Shader;

		std::vector<Texture*> m_Textures;

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		float m_Rotation;

		glm::mat4x4 m_ModelMatrix;
	public:
		Object2D(ArrayBuffer* buffer, Shader* shader, glm::mat4x4 model = glm::mat4(1.0f))
			: m_Buffer(buffer), m_Shader(shader), m_ModelMatrix(model), m_Position(0.0f), m_Scale(1.0f), m_Rotation(0.0f)
		{}

		Object2D(ArrayBuffer* buffer, Shader* shader, std::initializer_list<Texture*> textures, glm::mat4x4 model = glm::mat4(1.0f))
			: m_Buffer(buffer), m_Shader(shader), m_Textures(textures), m_ModelMatrix(model), m_Position(0.0f), m_Scale(1.0f), m_Rotation(0.0f)
		{
			for (int i = 0; i < m_Textures.size(); i++)
			{
				if(i < 32)
					m_Textures[i]->SetIndex(i);
				else
				{
					CR_GAME_ERROR("Cannot have more than 32 textures inside one object!"); 
					return;
				}
			}
		}

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; CalculateModelMatrix(); }
		const glm::vec3& GetScale() const { return m_Scale; }
		void SetScale(const glm::vec3& scale) { m_Scale = scale; CalculateModelMatrix(); }
		const float GetRotation() const { return m_Rotation; }
		void SetRotation(const float rotation) { m_Rotation = rotation; CalculateModelMatrix(); }

		const glm::mat4x4& GetModelMatrix() const { return m_ModelMatrix; }
		void SetModelMatrix(const glm::mat4x4& model) { m_ModelMatrix = model; }

		void AddTexture(Texture* texture)
		{
			if (m_Textures.size() < 32)
			{
				texture->SetIndex(m_Textures.size());
				m_Textures.push_back(texture);
			}
			else
			{
				CR_GAME_ERROR("Cannot have more than 32 textures inside one object!");
				return;
			}
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
	private:
		void CalculateModelMatrix()
		{
			m_ModelMatrix = 
			  glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), m_Scale);
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