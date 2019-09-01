#include "Shader.h"
#include "Crow/FileUtils.h"

#include <glad/glad.h>
#include <sstream>

namespace Crow {

	Shader::Shader(const char* path)
	{
		Init(FileUtils::ReadFile(path));
	}
		
	Shader::Shader(std::string shadercode)
	{
		Init(shadercode);
	}

	void Shader::CompileShader(const char* vertex, const char* fragment)
	{

		//SHADER
		uint vertexID = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexID, 1, &vertex, NULL);
		glCompileShader(vertexID);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
			CR_GAME_ERROR(infoLog);
		}

		//FRAGMENT

		uint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentID, 1, &fragment, NULL);
		glCompileShader(fragmentID);

		glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
			CR_GAME_ERROR(infoLog);
		}

		//LINKING

		m_ShaderID = glCreateProgram();
		glAttachShader(m_ShaderID, vertexID);
		glAttachShader(m_ShaderID, fragmentID);
		glLinkProgram(m_ShaderID);

		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ShaderID, 512, NULL, infoLog);
			CR_CORE_ERROR(infoLog);
		}
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
	}


	void Shader::Init(std::string fileSource)
	{
		ShaderType type = UKNOWN;

		std::string stringFragmentSource   = "";
		std::string stringVertexSource     = "";

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
			CR_GAME_ERROR("Source is not in a Crow Shader format! Use \"#shader fragment\" or \"#shader vertex\" to differentiate fragment and vertex code!");
			return;
		}

		CompileShader(stringVertexSource.c_str(), stringFragmentSource.c_str());
	}


	void Shader::Bind() const
	{
		glUseProgram(m_ShaderID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}