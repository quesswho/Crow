#pragma once

#include "Crow/Graphics/ShaderFactory.h"
#include "Crow/Graphics/PipelineStateObject.h"
#include "Crow/Graphics/Shader.h"
#include "Crow/Graphics/Texture.h"

#include <initializer_list>
#include <glm/gtc/matrix_transform.hpp>
#include "Crow/Collider/Rectangle.h"

#include "Crow/Application.h"

namespace Crow {

	class Object2D {
	public:
		const PipelineStateObject* m_PSO;
		std::vector<Texture*> m_Textures;

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		float m_Rotation;

		glm::mat4x4 m_ModelMatrix;

		Rectangle* m_Collider;

		bool m_IsShaderSpecified;
	public:
		Object2D(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, glm::vec3 position = glm::vec3(0.0f))
			: m_PSO(PipelineStateObject::Create(vBuffer, iBuffer, Shader::CreateFromSource("Object2DShader", Application::GetAPI()->GetShaderFactory()->ColorShader()))), 
			m_ModelMatrix(glm::mat4(1.0f)), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(false), m_Collider(0)
		{
			CalculateModelMatrix();
		}

		Object2D(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Texture* texture, glm::vec3 position = glm::vec3(0.0f)) // Only one texture for object without a specified shader!
			: m_PSO(PipelineStateObject::Create(vBuffer, iBuffer, Shader::CreateFromSource("Object2DShader", Application::GetAPI()->GetShaderFactory()->TextureShader()))), 
			m_ModelMatrix(glm::mat4(1.0f)), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(false), m_Collider(0)
		{
			m_Textures.push_back(texture);
			CalculateModelMatrix();
		}

		Object2D(const PipelineStateObject* pipelineStateObject, glm::vec3 position = glm::vec3(0.0f))
			: m_PSO(pipelineStateObject), m_ModelMatrix(glm::mat4(1.0f)), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(true), m_Collider(0)
		{
			CalculateModelMatrix();
		}

		Object2D(const PipelineStateObject* pipelineStateObject, Texture* texture, glm::vec3 position = glm::vec3(0.0f))
			: m_PSO(pipelineStateObject), m_ModelMatrix(glm::mat4(1.0f)), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(true), m_Collider(0)
		{
			if(texture != NULL)
				m_Textures.push_back(texture);
			CalculateModelMatrix();
		}

		Object2D(const PipelineStateObject* pipelineStateObject, std::vector<Texture*> textures, glm::vec3 position = glm::vec3(0.0f))
			: m_PSO(pipelineStateObject), m_Textures(textures), m_ModelMatrix(glm::mat4(1.0f)), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(true), m_Collider(0)
		{
			CalculateModelMatrix();
		}

		Object2D(const PipelineStateObject* pipelineStateObject, std::initializer_list<Texture*> textures, glm::vec3 position = glm::vec3(0.0f));

		~Object2D();

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline void SetPosition(const glm::vec3& position) { m_Position = position; CalculateModelMatrix();  UpdateCollider(); }
		inline void AddPosition(const glm::vec3& other) { m_Position += other; CalculateModelMatrix(); UpdateCollider(); }

		inline const glm::vec3& GetScale() const { return m_Scale; }
		inline void SetScale(const glm::vec3& scale) { m_Scale = scale; CalculateModelMatrix(); }
		inline void AddScale(const glm::vec3& other) { m_Scale += other; CalculateModelMatrix(); }

		inline const float GetRotation() const { return m_Rotation; }
		inline void SetRotation(const float rotation) { m_Rotation = rotation; CalculateModelMatrix(); }
		inline void AddRotation(const float other) { m_Rotation += other; CalculateModelMatrix(); }

		inline const glm::mat4x4& GetModelMatrix() const { return m_ModelMatrix; }
		inline void SetModelMatrix(const glm::mat4x4& model) { m_ModelMatrix = model; }

		inline void SetCollision(Rectangle* collision) { m_Collider = collision; m_Collider->m_Position.x = m_Position.x; m_Collider->m_Position.y = m_Position.y; }
		inline void SetCollision(glm::vec2 size) { m_Collider = new Rectangle(glm::vec2(size.x, size.y), glm::vec2(m_Position.x, m_Position.y)); }

		inline Rectangle* GetCollision() const { return m_Collider; }

		bool IsColliding(Rectangle* other) const;
		bool IsColliding(Object2D* other) const { return IsColliding(other->GetCollision()); }

		glm::vec2 GetCorrection(Rectangle* other) const;
		glm::vec2 GetCorrection(Object2D* other) const { return GetCorrection(other->GetCollision()); }

		void AddTexture(Texture* texture);


		const inline uint GetCount() const { return m_PSO->GetCount(); }

		void Bind() const;
		void Unbind() const;

	private:
		void UpdateCollider()
		{
			if (m_Collider != NULL)
			{
				m_Collider->m_Position.x = m_Position.x; m_Collider->m_Position.y = m_Position.y;
			}
		}
		void CalculateModelMatrix();
	};


	// Object2D class with arraybuffer replaced with vertices and indices
	struct Batchable2D {
		const uint m_Size;
		std::vector<uint> m_Indices;
		const uint m_Count;
		std::vector<float> m_Vertices;

		Shader* m_Shader;
		Texture* m_Texture;

		glm::vec3 m_Position;

		Batchable2D(const float* vertices, const uint size, const uint* indices, const uint count, glm::vec3 position = glm::vec3(0.0f))
			: m_Size(size), m_Count(count), m_Position(position), m_Shader(Shader::CreateFromSource("Object2DShader", Application::GetAPI()->GetShaderFactory()->ColorShader())), m_Texture(NULL)
		{
			m_Vertices = { vertices, vertices + size / sizeof(float) };
			m_Indices = { indices, indices + count };
		}

		Batchable2D(const float* vertices, const uint size, const uint* indices, const uint count, Shader* shader, glm::vec3 position = glm::vec3(0.0f))
			: m_Size(size), m_Count(count), m_Shader(shader), m_Position(position), m_Texture(NULL)
		{
			m_Vertices = { vertices, vertices + size / sizeof(float) };
			m_Indices = { indices, indices + count };
		}

		Batchable2D(const float* vertices, const uint size, const uint* indices, const uint count, Texture* texture, glm::vec3 position = glm::vec3(0.0f))
			: m_Size(size), m_Count(count), m_Texture(texture), m_Position(position), m_Shader(Shader::CreateFromSource("Object2DShader", Application::GetAPI()->GetShaderFactory()->TextureShader()))
		{
			m_Vertices = { vertices, vertices + size / sizeof(float) };
			m_Indices = { indices, indices + count };
		}

		Batchable2D(const float* vertices, const uint size, const uint* indices, const uint count, Shader* shader, Texture* texture, glm::vec3 position = glm::vec3(0.0f))
			: m_Size(size), m_Count(count), m_Shader(shader), m_Texture(texture), m_Position(position)
		{
			m_Vertices = { vertices, vertices + size / sizeof(float) };
			m_Indices = { indices, indices + count };
		}

		~Batchable2D()
		{
			m_Vertices.clear();
			m_Indices.clear();
		}
	};
}