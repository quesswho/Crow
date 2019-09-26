#include "DirectXShader.h"
#include "Crow/FileUtils.h"

#include <glad/glad.h>
#include <sstream>
#include <regex>

namespace Crow {
	namespace Platform {

		DirectXShader::DirectXShader(const char* name, const char* path)
			: m_Name(name)
		{
			Init(FileUtils::ReadFile(path));
		}

		DirectXShader::DirectXShader(const char* name, std::string& source)
			: m_Name(name)
		{
			Init(source);
		}

		DirectXShader::~DirectXShader()
		{
			
		}

		void DirectXShader::CompileShader(const char* vertex, const char* fragment)
		{
			HRESULT hr;
			ID3DBlob* errorBuff;
			bool failed = false;

			//VerteShader

			hr = D3DCompile(vertex,
				strlen(vertex),
				"Vertex Shader",
				nullptr,
				nullptr,
				"main",
				"vs_5_0", // Vertex Shader 5.0
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0,
				&m_VertexShader,
				&errorBuff);

			if (hr < 0)
			{
				CR_CORE_ERROR((char*)errorBuff->GetBufferPointer());
				failed = true;
			}

			hr = D3DCompile(fragment,
				strlen(fragment),
				"Fragment Shader",
				nullptr,
				nullptr,
				"main",
				"ps_5_0", // Fragment Shader 5.0
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0,
				&m_FragmentShader,
				&errorBuff);

			if (hr < 0)
			{
				CR_CORE_ERROR((char*)errorBuff->GetBufferPointer());
				failed = true;
			}

			if (!failed)
			{
				m_CompiledVertexShader = {};
				m_CompiledVertexShader.BytecodeLength = m_VertexShader->GetBufferSize();
				m_CompiledVertexShader.pShaderBytecode = m_VertexShader->GetBufferPointer();

				m_CompiledFragmentShader = {};
				m_CompiledFragmentShader.BytecodeLength = m_FragmentShader->GetBufferSize();
				m_CompiledFragmentShader.pShaderBytecode = m_FragmentShader->GetBufferPointer();
			}
		}


		void DirectXShader::Init(std::string& fileSource)
		{
			ShaderType type = UKNOWN;

			std::string stringFragmentSource = "";
			std::string stringVertexSource = "";

			std::istringstream source(fileSource);
			std::string line;

			int lineCount = 0;

			while (std::getline(source, line))
			{
				if (lineCount == 0 && line == "\"")
					continue;							// Get rid of quotation mark in beginning to allow highlightning

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
				lineCount++;
			}

			if (stringFragmentSource == "" || stringVertexSource == "")
			{
				CR_CORE_ERROR("{0}: Source is not in a Crow Shader format! Use \"#shader fragment\" or \"#shader vertex\" to differentiate between fragment shader code and vertex shader code!", m_Name);
				return;
			}

			CompileShader(stringVertexSource.c_str(), stringFragmentSource.c_str());
		}


		void DirectXShader::ReloadFromPath(const char* path)
		{
			Init(FileUtils::ReadFile(path));
		}

		void DirectXShader::ReloadFromSource(std::string& source)
		{
			Init(source);
		}

		void DirectXShader::Bind() const
		{
			
		}

		void DirectXShader::Unbind() const
		{
			
		}


		int DirectXShader::GetLocation(const char* location)
		{
			return 0;
		}

		void DirectXShader::SetUniform1i(const char* location, int value)
		{
			glUniform1i(GetLocation(location), value);
		}

		void DirectXShader::SetUniform1f(const char* location, float value)
		{
			glUniform1f(GetLocation(location), value);
		}
		void DirectXShader::SetUniform2f(const char* location, glm::vec2& value)
		{
			glUniform2f(GetLocation(location), value.x, value.y);
		}
		void DirectXShader::SetUniform3f(const char* location, glm::vec3& value)
		{
			glUniform3f(GetLocation(location), value.x, value.y, value.z);
		}
		void DirectXShader::SetUniform4f(const char* location, glm::vec4& value)
		{
			glUniform4f(GetLocation(location), value.x, value.y, value.z, value.w);
		}

		void DirectXShader::SetUniformMat4(const char* location, const glm::mat4x4& value)
		{
			glUniformMatrix4fv(GetLocation(location), 1, GL_FALSE, &value[0][0]);
		}
	}
}