#pragma once

#include "Crow/Graphics/Shader.h"

namespace Crow {
	namespace Platform {

		class OpenGLShader : public Shader {
		public:
			enum UniformType
			{
				UNKNOWN = -1,
				INT,
				FLOAT,
				FLOAT2,
				FLOAT3,
				FLOAT4,
				MAT3,
				MAT4
			};
		private:
			uint m_ShaderID;
			const char* m_Name;
			std::unordered_map<const char*, int> m_UniformLocations;
			//std::unordered_map<std::unordered_map<std::unordered_map<int, const char*>, const char*>, const char*> m_UniformStructs;	// <<>uniformlocation, uniformname>, uniformstructname>, uniformname>, 
		public:
			explicit OpenGLShader(const char* name, const char* path, const BufferProperties& shaderInput); // File path
			explicit OpenGLShader(const char* name, std::string& source, const BufferProperties& shaderInput); // Shader code

			~OpenGLShader() override;

			static Shader* CreateOpenGLShaderFromPath(const char* name, const char* path, const BufferProperties& shaderInput) { return new OpenGLShader(name, path, shaderInput); }
			static Shader* CreateOpenGLShaderFromSource(const char* name, std::string& source, const BufferProperties& shaderInput) { return new OpenGLShader(name, source, shaderInput); }

			virtual void Bind() const;
			virtual void Unbind() const;

			virtual void ReloadFromPath(const char* path) override;
			virtual void ReloadFromSource(std::string& source) override;

			virtual void CreateConstantBuffers(int frame) override {}

			virtual const char* GetName() const { return m_Name; }

			virtual void SetUniformValue(const char* location, int value) override;
			virtual void SetUniformValue(const char* location, float value) override;
			virtual void SetUniformValue(const char* location, glm::vec2& value) override;
			virtual void SetUniformValue(const char* location, glm::vec3& value) override;
			virtual void SetUniformValue(const char* location, glm::vec4& value) override;
			virtual void SetUniformValue(const char* location, const glm::mat3x3& value) override;
			virtual void SetUniformValue(const char* location, const glm::mat4x4& value) override;

			virtual void SetUniformStruct(const char* location, const void* data) override {}
		private:
			static UniformType StringToUniformType(std::string& source);

			void Init(std::string& fileSource);
			void CompileShader(const char* vertex, const char* fragment);
			void FindUniformStructs(std::string& source);

			int GetLocation(const char* location);
		};
	}
}