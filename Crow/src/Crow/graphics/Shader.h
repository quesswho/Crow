#pragma once

#include "Crow/Common.h"

namespace Crow {

	enum ShaderType { UKNOWN = -1, VERTEX = 0, FRAGMENT = 1 };

	class Shader {
	private:
		uint m_ShaderID;
	public:
		explicit Shader(const char* path); // File path
		explicit Shader(std::string shadercode); // Shader code

		~Shader() {}

		void Bind() const;
		void Unbind() const;

	private:
		void Init(std::string fileSource);
		void CompileShader(const char* vertex, const char* fragment);
	};
}