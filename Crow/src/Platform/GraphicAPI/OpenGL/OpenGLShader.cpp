#include "OpenGLShader.h"
#include "Crow/FileUtils.h"
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <sstream>
#include <regex>

namespace Crow {
	namespace Platform {

		OpenGLShader::OpenGLShader(const char* name, const char* path, const BufferProperties& shaderInput)
			: m_Name(name)
		{
			Init(FileUtils::ReadFile(path));
		}

		OpenGLShader::OpenGLShader(const char* name, std::string& source, const BufferProperties& shaderInput)
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

		void OpenGLShader::FindUniformStructs(std::string& source)
		{
			/*bool writing = false;
			std::string uniformStructName;

			std::unordered_map<std::unordered_map<std::string&, UniformType>, const char*> structUniforms;

			std::istringstream sourceStream(source);
			std::string line;

			while (std::getline(sourceStream, line))
			{
				if (line._Starts_with("struct"))
				{
					writing = true;
					line.erase(std::remove(line.begin(), line.end(), " {"), line.end());
					line.erase(std::remove(line.begin(), line.end(), "{"), line.end());
					uniformStructName = line.substr(strlen("cbuffer") + 1, line.length());

					std::unordered_map<std::string&, int> uniforms;

					while (std::getline(sourceStream, line) && !(line._Starts_with("}")) && writing)
					{
						line.erase(std::remove(line.begin(), line.end(), '\t'), line.end()); // Remove tab space

						while (line._Starts_with(" "))
							line.erase(std::remove(line.begin(), line.end(), ' '), line.end()); // Remove bad tab space

						line.erase(std::remove(line.begin(), line.end(), ';'), line.end());

						if (line == "{")
							continue;		// skip '{' if line is only that

						if (line._Starts_with("{"))
							line.replace(line.begin(), line.begin() + 1, "");  // Get rid of '{'

						uniforms.emplace(StringToUniformType(line.substr(0, line.find(" "))), line.substr(line.find(" ") + 1));

					}
					writing = false;		// End of cbuffer
					structUniforms.emplace(uniforms, uniformStructName);
				}
				for (auto& structUni : structUniforms)
				{
					if (line._Starts_with(structUni.second))
					{
						std::vector<int> locations;
						//locations.push_back()
					}
				}
			
			}*/
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
			if (m_UniformLocations.find(location) != m_UniformLocations.end())
			{
				return m_UniformLocations[location];
			}

			// New location
			const int locationi = glGetUniformLocation(m_ShaderID, location);
			m_UniformLocations[location] = locationi;
			return locationi;
		}

		void OpenGLShader::SetUniformValue(const char* location, int value)
		{
			glUniform1i(GetLocation(location), value);
		}

		void OpenGLShader::SetUniformValue(const char* location, float value)
		{
			glUniform1f(GetLocation(location), value);
		}
		void OpenGLShader::SetUniformValue(const char* location, glm::vec2& value)
		{
			glUniform2f(GetLocation(location), value.x, value.y);
		}
		void OpenGLShader::SetUniformValue(const char* location, glm::vec3& value)
		{
			glUniform3f(GetLocation(location), value.x, value.y, value.z);
		}
		void OpenGLShader::SetUniformValue(const char* location, glm::vec4& value)
		{
			glUniform4f(GetLocation(location), value.x, value.y, value.z, value.w);
		}
		void OpenGLShader::SetUniformValue(const char* location, const glm::mat3x3& value)
		{
			glUniformMatrix3fv(GetLocation(location), 1, GL_FALSE, &value[0][0]);
		}
		void OpenGLShader::SetUniformValue(const char* location, const glm::mat4x4& value)
		{
			glUniformMatrix3fv(GetLocation(location), 1, GL_FALSE, &value[0][0]);
		}
	}
}