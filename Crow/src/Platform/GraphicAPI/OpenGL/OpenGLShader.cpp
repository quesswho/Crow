#include "OpenGLShader.h"
#include "Crow/FileUtils.h"

#include <glad/glad.h>
#include <sstream>
#include <regex>
#include <algorithm>

namespace Crow {
	namespace Platform {

		OpenGLShader::OpenGLShader(const char* name, const char* path, const BufferProperties& shaderInput)
			: m_Name(name)
		{
			uint len = strlen(path);
			if (path[len-1] == 'x' && path[len - 2] == '.')
			{
				len += strlen("lsl");
				char* newpath = new char[len + 1];
				*newpath = '\0';
				strcat(newpath, path);
				newpath[strlen(path)-1] = 'g';
				Init(FileUtils::ReadFile(strcat(newpath, "lsl")));
				delete newpath;
			}
			else
			{
				Init(FileUtils::ReadFile(path));
			}
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
			ShaderType type = ShaderType::UNKNOWN;

			std::string stringFragmentSource = "";
			std::string stringVertexSource = "";

			std::istringstream source(fileSource);
			std::string line;
			while (std::getline(source, line))
			{
				if (line == "#shader fragment")
				{
					type = ShaderType::FRAGMENT;
					continue;
				}

				if (line == "#shader vertex")
				{
					type = ShaderType::VERTEX;
					continue;
				}

				switch (type) {
				case ShaderType::FRAGMENT:
					stringFragmentSource.append(line).append("\n");
					break;
				case ShaderType::VERTEX:
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
			FindUniformStructs(stringVertexSource.append("\n").append(stringFragmentSource));
		}

		void OpenGLShader::FindUniformStructs(std::string& source)
		{
			bool writing = false;
			std::string uniformStructName;

			std::vector<TempShaderUniformStruct> structs; // struct name, uniforms, size

			std::istringstream sourceStream(source);
			std::string line;

			int size = 0;

			while (std::getline(sourceStream, line))
			{
				if (line._Starts_with("struct"))
				{
					writing = true;
					line.erase(std::remove(line.begin(), line.end(), '{'), line.end());
					while (line.back() == ' ') line = line.substr(0, line.size() - 1); // Remove space at end

					//uniformStructName = line.substr(strlen("struct") + 1, line.length());
					
					uniformStructName = std::string_view(line.c_str() + strlen("struct") + 1, line.length() - strlen("struct") - 1);

					std::vector<TempUniform> uniforms; // Name, size

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

						
						size += StringToUniformTypeSize(std::string_view(line.c_str(), line.find(" ")));
						
						uniforms.push_back(TempUniform(line.substr(line.find(" ") + 1, line.size()), StringToUniformType(std::string_view(line.c_str(), line.find(" ")))));
					}
					writing = false;		// End of cbuffer
					structs.push_back(TempShaderUniformStruct(uniformStructName, uniforms, size));
					size = 0;
				}
				else if (line._Starts_with("uniform"))
				{
					while (line.back() == ' ') line = line.substr(0, line.size() - 1); // Remove space at end

					line = line.substr(strlen("uniform") + 1, line.size());
					std::string structtype = line.substr(0, line.find(" "));
					line.erase(std::remove(line.begin(), line.end(), ';'), line.end());

					std::string uniformStructName = line.substr(strlen(structtype.c_str()) + 1).c_str();

					for (auto& structUni : structs)
					{
						if (line._Starts_with(structUni.m_Name))
						{
							std::vector<Uniform> newUniforms;
							for (auto& uni : structUni.m_Uniforms)
							{
								newUniforms.push_back(Uniform(glGetUniformLocation(m_ShaderID, std::string(uniformStructName).append(".").append(uni.m_Name).c_str()), uni.m_Type));
							}

							m_UniformStructs.push_back(ShaderUniformStruct(line.substr(strlen(structtype.c_str()) + 1), newUniforms, structUni.m_Size));
							m_UniformStructLocations.emplace(line.substr(strlen(structtype.c_str()) + 1), m_UniformStructLocations.size()); // Cache the location of each uniformstruct
						}
					}
				}
			}
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

		void OpenGLShader::SetUniformValue(const char* location, const int value)
		{
			glUniform1i(GetLocation(location), value);
		}

		void OpenGLShader::SetUniformValue(const char* location, const float value)
		{
			glUniform1f(GetLocation(location), value);
		}
		void OpenGLShader::SetUniformValue(const char* location, const Crow::Math::TVec2<float>& value)
		{
			glUniform2f(GetLocation(location), value.x, value.y);
		}
		void OpenGLShader::SetUniformValue(const char* location, const Crow::Math::TVec3<float>& value)
		{
			glUniform3f(GetLocation(location), value.x, value.y, value.z);
		}
		void OpenGLShader::SetUniformValue(const char* location, const Crow::Math::TVec4<float>& value)
		{
			glUniform4f(GetLocation(location), value.x, value.y, value.z, value.w);
		}
		void OpenGLShader::SetUniformValue(const char* location, const Math::Mat2& value)
		{
			glUniformMatrix2fv(GetLocation(location), 1, GL_FALSE, &value.m_Elements[0]);
		}
		void OpenGLShader::SetUniformValue(const char* location, const Math::Mat3& value)
		{
			glUniformMatrix3fv(GetLocation(location), 1, GL_FALSE, &value.m_Elements[0]);
		}
		void OpenGLShader::SetUniformValue(const char* location, const Math::Mat4& value)
		{
			glUniformMatrix4fv(GetLocation(location), 1, GL_FALSE, &value.m_Elements[0]);
		}

		void OpenGLShader::SetUniformStruct(const char* location, void* data)
		{
 			if (m_UniformStructLocations.find(location) != m_UniformStructLocations.end())
			{
				uchar* bytes = reinterpret_cast<uchar*>(data);
				for (Uniform uni : m_UniformStructs[((int)m_UniformStructLocations.at(location))].m_Uniforms)
				{
					switch (uni.m_Type)
					{
					case UniformType::INT:
						glUniform1iv(uni.m_Location, 1, (const GLint*)bytes);
						bytes += 4;
						break;
					case UniformType::FLOAT:
						glUniform1fv(uni.m_Location, 1, (const GLfloat*)bytes);
						bytes += 4;
						break;
					case UniformType::FLOAT2:
						glUniform2fv(uni.m_Location, 1, (const GLfloat*)bytes);
						bytes += 8;
						break;
					case UniformType::FLOAT3:
						glUniform3fv(uni.m_Location, 1, (const GLfloat*)bytes);
						bytes += 12;
						break;
					case UniformType::FLOAT4:
						glUniform4fv(uni.m_Location, 1, (const GLfloat*)bytes);
						bytes += 16;
						break;
					case UniformType::MAT2:
						glUniformMatrix2fv(uni.m_Location, 1, GL_FALSE, (const GLfloat*)bytes);
						bytes += 16;
						break;
					case UniformType::MAT3:
						glUniformMatrix3fv(uni.m_Location, 1, GL_FALSE, (const GLfloat*)bytes);
						bytes += 36;
						break;
					case UniformType::MAT4:
						glUniformMatrix4fv(uni.m_Location, 1, GL_FALSE, (const GLfloat*)bytes);
						bytes += 64;
						break;
					}
				}
			}
			else
			{
				CR_CORE_WARNING("\"{}\" Uniform structure not found!", location);
			}
		}

		int OpenGLShader::StringToUniformTypeSize(std::string_view type)
		{
			if (type == "float" 
				|| type == "int")	return 4;
			if (type == "vec2")		return 2 * 4;
			if (type == "vec3")		return 3 * 4;
			if (type == "vec4")		return 4 * 4;
			if (type == "mat2")		return 2 * 2 * 4;
			if (type == "mat3")		return 3 * 3 * 4;
			if (type == "mat4")		return 4 * 4 * 4;
			return -1;
		}

		Shader::UniformType OpenGLShader::StringToUniformType(std::string_view type)
		{
			if (type == "int")		return UniformType::INT;
			if (type == "float")	return UniformType::FLOAT;
			if (type == "vec2")		return UniformType::FLOAT2;
			if (type == "vec3")		return UniformType::FLOAT3;
			if (type == "vec4")		return UniformType::FLOAT4;
			if (type == "mat2")		return UniformType::MAT2;
			if (type == "mat3")		return UniformType::MAT3;
			if (type == "mat4")		return UniformType::MAT4;
			return OpenGLShader::UniformType::UNKNOWN;
		}

		int OpenGLShader::UniformTypeToUniformTypeSize(Shader::UniformType type)
		{
			switch (type)
			{
				case UniformType::FLOAT:
				case UniformType::INT:
					return 4;
				case UniformType::FLOAT2:
					return 2 * 4;
				case UniformType::FLOAT3:
					return 3 * 4;
				case UniformType::FLOAT4: // 4 * 4 = 2 * 2 * 4
				case UniformType::MAT2:
					return 2 * 2 * 4;
				case UniformType::MAT3:
					return 3 * 3 * 4;
				case UniformType::MAT4:
					return 4 * 4 * 4;
				default:
					return -1;
			}
		}
	}
}