#include "Object2D.h"

namespace Crow {

	Object2D::Object2D(const ArrayBuffer* pipelineStateObject, Shader* shader, std::initializer_list<Texture*> textures, glm::vec3 position)
		: m_ArrayBuffer(pipelineStateObject), m_Shader(shader), m_Textures(textures), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(true), m_ModelMatrix(glm::mat4(1.0f)), m_Collider(0)
	{
		for (int i = 0; i < m_Textures.size(); i++)
		{
			if (i < 32)
				m_Textures[i]->SetIndex(i);
			else
			{
				CR_CORE_ERROR("Cannot have more than 32 textures inside one object!");
				return;
			}
		}
		CalculateModelMatrix();
	}

	Object2D::~Object2D()
	{
		delete m_Collider;
		delete m_ArrayBuffer;
	}

	void Object2D::AddTexture(Texture* texture)
	{
		if (m_IsShaderSpecified)
		{
			if (m_Textures.size() < 32)
			{
				texture->SetIndex((const int)m_Textures.size());
				m_Textures.push_back(texture);
			}
			else
			{
				CR_CORE_ERROR("Cannot have more than 32 textures inside one object!");
				return;
			}
		}
		else
		{
			if (m_Textures.size() < 1)
			{
				m_Shader->ReloadFromSource(Application::GetAPI()->GetShaderFactory()->TextureShader());
				texture->SetIndex(0);
				m_Shader->SetUniform1i("u_Texture", 0);
				m_Textures.push_back(texture);
			}
			else
			{
				CR_CORE_ERROR("Cannot have more than 1 textures inside an object without a specified shader!");
				return;
			}
		}
	}

	bool Object2D::IsColliding(Rectangle* other) const
	{
		return m_Collider->IsCollide(other);
	}

	glm::vec2 Object2D::GetCorrection(Rectangle* other) const
	{
		return m_Collider->GetCorrection(other);
	}

	void Object2D::Bind() const
	{
		for (int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->Bind();
		}
		m_ArrayBuffer->Bind();
	}

	void Object2D::Unbind() const
	{
		for (int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->Unbind();
		}
		m_ArrayBuffer->Bind();
	}

	void Object2D::CalculateModelMatrix()
	{
		m_ModelMatrix =
			glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), m_Scale);
	}
}