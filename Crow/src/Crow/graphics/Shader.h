#pragma once
#include "Crow/Common.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Crow {


	enum ShaderType { UKNOWN = -1, VERTEX = 0, FRAGMENT = 1 };

	struct Shader {

		virtual ~Shader() = default;

		static Shader* CreateFromPath(const char* name, const char* path);
		static Shader* CreateFromSource(const char* name, std::string& source);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void ReloadFromPath(const char* path) = 0;
		virtual void ReloadFromSource(std::string& source) = 0;

		virtual const char* GetName() const = 0;

		virtual void SetUniform1i(const char* location, int value) = 0;
		virtual void SetUniform1f(const char* location, float value) = 0;
		virtual void SetUniform2f(const char* location, glm::vec2& value) = 0;
		virtual void SetUniform3f(const char* location, glm::vec3& value) = 0;
		virtual void SetUniform4f(const char* location, glm::vec4& value) = 0;
		virtual void SetUniformMat4(const char* location, const glm::mat4x4& value) = 0;
	};
}