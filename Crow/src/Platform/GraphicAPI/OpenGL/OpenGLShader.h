#pragma once

#include "Crow/Graphics/Shader.h"

namespace Crow {
	namespace Platform {

		class OpenGLShader : public Shader {
		private:
			uint m_ShaderID;
			const char* m_Name;
			std::unordered_map<const char*, int> m_UniformLocations;
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

			virtual const char* GetName() const { return m_Name; }

			virtual void SetUniform1i(const char* location, int value);
			virtual void SetUniform1f(const char* location, float value);
			virtual void SetUniform2f(const char* location, glm::vec2& value);
			virtual void SetUniform3f(const char* location, glm::vec3& value);
			virtual void SetUniform4f(const char* location, glm::vec4& value);
			virtual void SetUniformMat4(const char* location, const glm::mat4x4& value);
		private:
			void Init(std::string& fileSource);
			void CompileShader(const char* vertex, const char* fragment);

			int GetLocation(const char* location);
		};
	}
}