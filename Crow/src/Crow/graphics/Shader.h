#pragma once

#include "Crow/Common.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Crow {


	enum ShaderType { UKNOWN = -1, VERTEX = 0, FRAGMENT = 1 };

	class Shader {
		uint m_ShaderID;
		const char* m_Name;
		std::vector<std::tuple<const char*, int>> m_UniformLocations;
	public:
		explicit Shader(const char* name, const char* path); // File path
		explicit Shader(const char* name, std::string& shadercode); // Shader code

		~Shader() {}

		void Bind() const;
		void Unbind() const;

		const char* GetName() noexcept { return m_Name; }

		void SetUniform1i(const char* location, int value);
		void SetUniform1f(const char* location, float value);
		void SetUniform2f(const char* location, glm::vec2& value);
		void SetUniform3f(const char* location, glm::vec3& value);
		void SetUniform4f(const char* location, glm::vec4& value);
		void SetUniformMat4(const char* location, glm::mat4x4& value);
	private:
		void Init(std::string& fileSource);
		void CompileShader(const char* vertex, const char* fragment);


		int GetLocation(const char* location);
	};
}