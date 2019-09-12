#include "OpenGLShader.h"
#include "Crow/FileUtils.h"

#include <glad/glad.h>
#include <sstream>
#include <regex>

namespace Crow {
	namespace Platform {

		OpenGLShader::OpenGLShader(const char* name, const char* path)
			: m_Name(name)
		{
			Init(FileUtils::ReadFile(path));
		}

		OpenGLShader::OpenGLShader(const char* name, std::string& source)
			: m_Name(name)
		{
			Init(source);
		}

		OpenGLShader::~OpenGLShader()
		{
			glDeleteProgram(m_ShaderID);
		}

		void OpenGLShader::CompileShader(const char* vertex, const char* fragment)
		{
			//SHADER
			uint vertexID = glCreateShader(GL_VERTEX_SHADER);

			glShaderSource(vertexID, 1, &vertex, NULL);
			glCompileShader(vertexID);

			int success;
			char infoLog[512];
			bool failed = false;
			glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
				std::string stringlog = "Vertex Shader: ";
				stringlog.append(infoLog);
				stringlog = std::regex_replace(stringlog, std::regex("\\ERROR:"), m_Name);
				CR_CORE_ERROR(stringlog);
				failed = true;
			}

			//FRAGMENT

			uint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(fragmentID, 1, &fragment, NULL);
			glCompileShader(fragmentID);

			glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
				std::string stringlog = "Fragment Shader: ";
				stringlog.append(infoLog);
				stringlog = std::regex_replace(stringlog, std::regex("\\ERROR:"), m_Name);
				CR_CORE_ERROR(stringlog);
				failed = true;
			}


			if (failed) return; // Get error from both shaders

			//LINKING

			m_ShaderID = glCreateProgram();
			glAttachShader(m_ShaderID, vertexID);
			glAttachShader(m_ShaderID, fragmentID);
			glLinkProgram(m_ShaderID);

			glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_ShaderID, 512, NULL, infoLog);
				std::string stringlog = infoLog;
				stringlog.append("Linking Shader: ");
				stringlog = std::regex_replace(stringlog, std::regex("\\Error:"), m_Name);
				CR_CORE_ERROR(stringlog);
				return;
			}
			glDeleteShader(vertexID);
			glDeleteShader(fragmentID);
		}


		void OpenGLShader::Init(std::string& fileSource)
		{
			ShaderType type = UKNOWN;

			std::string stringFragmentSource = "";
			std::string stringVertexSource = "";

			std::istringstream source(fileSource);
			std::string line;
			while (std::getline(source, line))
			{
				if (line == "#shader fragment")
				{
					type = FRAGMENT;
					continue;
				}

				if (line == "#shader vertex")
				{
					type = VERTEX;
					continue;
				}

				switch (type) {
				case FRAGMENT:
					stringFragmentSource.append(line).append("\n");
					break;
				case VERTEX:
					stringVertexSource.append(line).append("\n");
					break;
				}
			}

			if (stringFragmentSource == "" || stringVertexSource == "")
			{
				CR_CORE_ERROR("{0}: Source is not in a Crow Shader format! Use \"#shader fragment\" or \"#shader vertex\" to differentiate between fragment shader code and vertex shader code!", m_Name);
				return;
			}

			CompileShader(stringVertexSource.c_str(), stringFragmentSource.c_str());
		}


		void OpenGLShader::ReloadFromPath(const char* path)
		{
			Init(FileUtils::ReadFile(path));
		}

		void OpenGLShader::ReloadFromSource(std::string& source)
		{
			Init(source);
		}

		void OpenGLShader::Bind() const
		{
			glUseProgram(m_ShaderID);
		}

		void OpenGLShader::Unbind() const
		{
			glUseProgram(0);
		}


		int OpenGLShader::GetLocation(const char* location)
		{
			if (m_UniformLocations[location] != std::string::npos)
			{
				return m_UniformLocations[location];
			}

			// New location
			const int locationi = glGetUniformLocation(m_ShaderID, location);
			m_UniformLocations[location] = locationi;
			return locationi;
		}

		void OpenGLShader::SetUniform1i(const char* location, int value)
		{
			glUniform1i(GetLocation(location), value);
		}

		void OpenGLShader::SetUniform1f(const char* location, float value)
		{
			glUniform1f(GetLocation(location), value);
		}
		void OpenGLShader::SetUniform2f(const char* location, glm::vec2& value)
		{
			glUniform2f(GetLocation(location), value.x, value.y);
		}
		void OpenGLShader::SetUniform3f(const char* location, glm::vec3& value)
		{
			glUniform3f(GetLocation(location), value.x, value.y, value.z);
		}
		void OpenGLShader::SetUniform4f(const char* location, glm::vec4& value)
		{
			glUniform4f(GetLocation(location), value.x, value.y, value.z, value.w);
		}

		void OpenGLShader::SetUniformMat4(const char* location, const glm::mat4x4& value)
		{
			glUniformMatrix4fv(GetLocation(location), 1, GL_FALSE, &value[0][0]);
		}
	}
}