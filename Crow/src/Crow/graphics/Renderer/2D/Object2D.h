#pragma once

#include "Crow/Graphics/ShaderFactory.h"
#include "Crow/Graphics/Renderer/ArrayBuffer.h"
#include "Crow/Graphics/Shader.h"
#include "Crow/Graphics/Texture.h"

#include <initializer_list>
#include <glm/gtc/matrix_transform.hpp>

namespace Crow {

	class Object2D {
	public:
		const ArrayBuffer* m_Buffer;
		Shader* m_Shader;
		std::vector<Texture*> m_Textures;

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		float m_Rotation;

		glm::mat4x4 m_ModelMatrix;

		bool m_IsShaderSpecified;
	public:
		Object2D(const ArrayBuffer* buffer, glm::vec3 position = glm::vec3(0.0f))
			: m_Buffer(buffer), m_Shader(Shader::CreateFromSource("Object2DShader", ShaderFactory::ColorShader())), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(false)
		{
			CalculateModelMatrix();
		}

		Object2D(const ArrayBuffer* buffer, Texture* texture, glm::vec3 position = glm::vec3(0.0f)) // Only one texture for object without a specified shader!
			: m_Buffer(buffer), m_Shader(Shader::CreateFromSource("Object2DShader", ShaderFactory::ColorShader())),  m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(false)
		{
			m_Textures.push_back(texture);
			CalculateModelMatrix();
		}

		Object2D(const ArrayBuffer* buffer, Shader* shader, glm::vec3 position = glm::vec3(0.0f))
			: m_Buffer(buffer), m_Shader(shader), m_ModelMatrix(glm::mat4(1.0f)), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(true)
		{
			CalculateModelMatrix();
		}

		Object2D(const ArrayBuffer* buffer, Shader* shader, Texture* texture, glm::vec3 position = glm::vec3(0.0f))
			: m_Buffer(buffer), m_Shader(shader), m_ModelMatrix(glm::mat4(1.0f)), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(true)
		{
			if(texture != NULL)
				m_Textures.push_back(texture);
			CalculateModelMatrix();
		}

		Object2D(const ArrayBuffer* buffer, Shader* shader, std::vector<Texture*> textures, glm::vec3 position = glm::vec3(0.0f))
			: m_Buffer(buffer), m_Shader(shader), m_Textures(textures), m_ModelMatrix(glm::mat4(1.0f)), m_Position(position), m_Scale(1.0f), m_Rotation(0.0f), m_IsShaderSpecified(true)
		{
			CalculateModelMatrix();
		}

		Object2D(const ArrayBuffer* buffer, Shader* shader, std::initializer_list<Texture*> textures, glm::vec3 position = glm::vec3(0.0f));

		~Object2D();

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline void SetPosition(const glm::vec3& position) { m_Position = position; CalculateModelMatrix(); }
		inline void AddPosition(const glm::vec3& other) { m_Position += other; CalculateModelMatrix(); }

		inline const glm::vec3& GetScale() const { return m_Scale; }
		inline void SetScale(const glm::vec3& scale) { m_Scale = scale; CalculateModelMatrix(); }
		inline void AddScale(const glm::vec3& other) { m_Scale += other; CalculateModelMatrix(); }

		inline const float GetRotation() const { return m_Rotation; }
		inline void SetRotation(const float rotation) { m_Rotation = rotation; CalculateModelMatrix(); }
		inline void AddRotation(const float other) { m_Rotation += other; CalculateModelMatrix(); }

		inline const glm::mat4x4& GetModelMatrix() const { return m_ModelMatrix; }
		inline void SetModelMatrix(const glm::mat4x4& model) { m_ModelMatrix = model; }

		void AddTexture(Texture* texture);


		const inline uint GetCount() const { return m_Buffer->GetCount(); }

		void Bind() const;
		void Unbind() const;

	private:
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
			: m_Size(size), m_Count(count), m_Position(position), m_Shader(Shader::CreateFromSource("Object2DShader", ShaderFactory::ColorShader())), m_Texture(NULL)
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
			: m_Size(size), m_Count(count), m_Texture(texture), m_Position(position), m_Shader(Shader::CreateFromSource("Object2DShader", ShaderFactory::TextureShader()))
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