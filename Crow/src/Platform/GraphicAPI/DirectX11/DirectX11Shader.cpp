#include "DirectX11Shader.h"
#include "Crow/FileUtils.h"

#include <d3dcompiler.h>
#include <sstream>

namespace Crow {
	namespace Platform {

		DirectX11Shader::DirectX11Shader(const char* name, const char* path, const BufferProperties& shaderInput)
			: m_Name(name), m_ShaderInput(shaderInput) //TODO: Automatically decide the shader input
		{
			uint len = strlen(path);
			if (path[len - 1] == 'x' && path[len - 2] == '.')
			{
				len += strlen("lsl");
				char* newpath = new char[len];
				*newpath = '\0';
				strcat(newpath, path);
				newpath[strlen(path) - 1] = 'h';
				Init(FileUtils::ReadFile(strcat(newpath, "lsl")));
			}
			else
			{
				Init(FileUtils::ReadFile(path));
			}
		}

		DirectX11Shader::DirectX11Shader(const char* name, std::string& source, const BufferProperties& shaderInput)
			: m_Name(name), m_ShaderInput(shaderInput)
		{
			Init(source);
		}

		DirectX11Shader::~DirectX11Shader()
		{
			m_FragmentShader->Release();
			m_VertexShader->Release();
			m_CompiledFragmentShader->Release();
			m_CompiledVertexShader->Release();
			m_VertexInputLayout->Release();
			for (auto cb : m_ConstanBuffers)
				delete cb;
		}


		void DirectX11Shader::Init(std::string& fileSource)
		{
			ShaderType type = ShaderType::UNKNOWN;

			std::string stringFragmentSource = "";
			std::string stringVertexSource = "";

			std::istringstream source(fileSource);
			std::string line;

			bool firstLine = true;

			while (std::getline(source, line))
			{
				if (firstLine)
				{
					firstLine = false;
					if (line == "\"") continue;	// Get rid of quotation mark in beginning to allow highlightning
				}

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

			FindConstantBuffer(stringVertexSource, stringFragmentSource);
			CompileShader(stringVertexSource.c_str(), stringFragmentSource.c_str());
			InitPipeline();
		}

		void DirectX11Shader::FindConstantBuffer(std::string& vertex, std::string& fragment)
		{
			bool writing = false;
			UniformType type = UniformType::UNKNOWN;
			std::string name;
			std::string cbuffername;

			int reg = 0;

			std::istringstream source(vertex);
			std::string line;

			int size = 0;
			while (std::getline(source, line))
			{
				if (line._Starts_with("cbuffer"))
				{
					writing = true;
					reg = (int)line.find(':'); // Using reg as a temporary value before setting register value
					if (reg != std::string::npos)
					{
						cbuffername = line.substr(strlen("cbuffer") + 1, reg - strlen("cbuffer") - 2);
						reg = (int)line.find("register(b", reg);
						if (reg != std::string::npos)
						{
							reg = std::stoi(line.substr((((size_t)reg + 10)), line.size() - reg - 11)); // Convert to int between "register(b" and ')'
							size = 0;
						}
					}

					std::unordered_map<UniformType, std::string> uniforms;

					while (std::getline(source, line) && !(line._Starts_with("}")) && writing)
					{
						line.erase(std::remove(line.begin(), line.end(), '\t'), line.end()); // Remove tab space

						while (line._Starts_with(" "))
							line.erase(line.begin()); // Remove bad tab space

						line.erase(std::remove(line.begin(), line.end(), ';'), line.end());

						if (line == "{")
							continue;		// skip '{' if line is only that

						if (line._Starts_with("{"))
							line.replace(line.begin(), line.begin() + 1, "");  // Get rid of '{'


						type = StringToUniformType(line.substr(0, line.find(" ")));
						name = line.substr(line.find(" ") + 1);
						uniforms.emplace(type, name);
						size += UniformTypeToSize(type);
					}
					writing = false;		// End of cbuffer

					m_ConstanBuffers.push_back(new DX11ConstantBuffer(size, reg, ShaderType::VERTEX));
					m_ConstantBufferLocations.emplace(cbuffername, m_ConstanBuffers.size() - 1);
					uniforms.clear();
				}
			}

			source = std::istringstream(fragment);

			while (std::getline(source, line))
			{
				if (line._Starts_with("cbuffer"))
				{
					writing = true;
					reg = (int)line.find(':'); // Using reg as a temporary value before setting register value
					if (reg != std::string::npos)
					{
						cbuffername = line.substr(strlen("cbuffer") + 1, reg - strlen("cbuffer") - 2);
						reg = (int)line.find("register(b", reg);
						if (reg != std::string::npos)
						{
							reg = std::stoi(line.substr((size_t)reg + 10, line.size() - reg - 11)); // Convert to int between "register(b" and ')'
							size = 0;
						}
					}

					std::unordered_map<UniformType, std::string> uniforms;

					while (std::getline(source, line) && !(line._Starts_with("}")) && writing)
					{
						line.erase(std::remove(line.begin(), line.end(), '\t'), line.end()); // Remove tab space

						while (line._Starts_with(" "))
							line.erase(line.begin()); // Remove bad tab space

						line.erase(std::remove(line.begin(), line.end(), ';'), line.end());

						if (line == "{")
							continue;		// skip '{' if line is only that

						if (line._Starts_with("{"))
							line.replace(line.begin(), line.begin() + 1, "");  // Get rid of '{'


						type = StringToUniformType(line.substr(0, line.find(" ")));
						name = line.substr(line.find(" ") + 1);
						uniforms.emplace(type, name);
						size += UniformTypeToSize(type);
					}
					writing = false;		// End of cbuffer
					m_ConstanBuffers.push_back(new DX11ConstantBuffer(size, reg, ShaderType::FRAGMENT));
					m_ConstantBufferLocations.emplace(cbuffername, m_ConstanBuffers.size() - 1);
					uniforms.clear();
				}
			}
		}


		void DirectX11Shader::ReloadFromPath(const char* path)
		{
			Init(FileUtils::ReadFile(path));
		}

		void DirectX11Shader::ReloadFromSource(std::string& source)
		{
			Init(source);
		}

		bool DirectX11Shader::CompileShader(const char* vertex, const char* fragment)
		{
			HRESULT hr;

			ID3DBlob* errorBuff;
			bool failed = false;

#if defined(CR_DEBUG)
			UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			UINT compileFlags = 0;
#endif

			//Vertex Shader

			hr = D3DCompile(vertex,
				strlen(vertex),
				"Vertex Shader",
				nullptr,
				nullptr,
				"VSmain",
				"vs_4_0", // Vertex Shader Model 4.0
				compileFlags,
				0,
				&m_CompiledVertexShader,
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
				"PSmain",
				"ps_4_0", // Fragment Shader Model 4.0
				compileFlags,
				0,
				&m_CompiledFragmentShader,
				&errorBuff);

			if (hr < 0)
			{
				CR_CORE_ERROR((char*)errorBuff->GetBufferPointer());
				failed = true;
			}

			if (!failed)
			{
				DirectX11RenderAPI::GetDevice()->CreateVertexShader(m_CompiledVertexShader->GetBufferPointer(), m_CompiledVertexShader->GetBufferSize(), NULL, &m_VertexShader);
				DirectX11RenderAPI::GetDevice()->CreatePixelShader(m_CompiledFragmentShader->GetBufferPointer(), m_CompiledFragmentShader->GetBufferSize(), NULL, &m_FragmentShader);
				return true;
			}
			return false;
		}

		void DirectX11Shader::InitPipeline()
		{
			if (m_ShaderInput.m_Stride > 0)
			{
				const uint size = (uint)m_ShaderInput.m_Elements.size();
				D3D11_INPUT_ELEMENT_DESC* inputLayout = new D3D11_INPUT_ELEMENT_DESC[size];

				for (uint i = 0; i < size; i++)
				{
					BufferElement element = m_ShaderInput.m_Elements[i];
					inputLayout[i] = { element.GetName(), 0, ConvertToDXGIFormat(element.GetComponentCount()), 0, (uint)element.GetOffset() * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				}
				DirectX11RenderAPI::GetDevice()->CreateInputLayout(inputLayout, (uint) m_ShaderInput.m_Elements.size(), m_CompiledVertexShader->GetBufferPointer(), m_CompiledVertexShader->GetBufferSize(), &m_VertexInputLayout);
			}

			// Constant Buffers

			for (auto cb : m_ConstanBuffers)
			{
				D3D11_BUFFER_DESC cbDesc;
				cbDesc.ByteWidth = (cb->m_Size + 15) & ~15;
				cbDesc.Usage = D3D11_USAGE_DEFAULT;
				cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				cbDesc.CPUAccessFlags = 0;
				cbDesc.MiscFlags = 0;

				DirectX11RenderAPI::GetDevice()->CreateBuffer(&cbDesc, NULL, &cb->m_ConstantBuffer);
			}

		}

		void DirectX11Shader::Bind() const
		{
			DirectX11RenderAPI::GetDeviceContext()->VSSetShader(m_VertexShader, 0, 0);
			DirectX11RenderAPI::GetDeviceContext()->PSSetShader(m_FragmentShader, 0, 0);
			DirectX11RenderAPI::GetDeviceContext()->IASetInputLayout(m_VertexInputLayout);
		}

		void DirectX11Shader::Unbind() const
		{}


		int DirectX11Shader::GetLocation(const char* location)
		{
#ifdef CR_RELEASE
			return m_ConstantBufferLocations.at(location);
#elif CR_DEBUG
			if (m_ConstantBufferLocations.find(location) != m_ConstantBufferLocations.end())
				return m_ConstantBufferLocations.at(location);
			else
			{
				CR_CORE_FATAL("\"{}\" Uniform not found!", location);
				return 0;
			}
#endif
		}

		void DirectX11Shader::SetUniformValue(const char* location, int value)
		{
			DX11ConstantBuffer* cb = m_ConstanBuffers[GetLocation(location)];
			DirectX11RenderAPI::GetDeviceContext()->UpdateSubresource(cb->m_ConstantBuffer, 0, NULL, &value, 0, 0);
			if (cb->m_Type == ShaderType::VERTEX)
				DirectX11RenderAPI::GetDeviceContext()->VSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
			else
				DirectX11RenderAPI::GetDeviceContext()->PSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
		}

		void DirectX11Shader::SetUniformValue(const char* location, float value)
		{
			DX11ConstantBuffer* cb = m_ConstanBuffers[GetLocation(location)];
			DirectX11RenderAPI::GetDeviceContext()->UpdateSubresource(cb->m_ConstantBuffer, 0, NULL, &value, 0, 0);
			if (cb->m_Type == ShaderType::VERTEX)
				DirectX11RenderAPI::GetDeviceContext()->VSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
			else
				DirectX11RenderAPI::GetDeviceContext()->PSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
		}
		void DirectX11Shader::SetUniformValue(const char* location, const Crow::Math::TVec2<float>& value)
		{
			DX11ConstantBuffer* cb = m_ConstanBuffers[GetLocation(location)];
			DirectX11RenderAPI::GetDeviceContext()->UpdateSubresource(cb->m_ConstantBuffer, 0, NULL, &value, 0, 0);
			if (cb->m_Type == ShaderType::VERTEX)
				DirectX11RenderAPI::GetDeviceContext()->VSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
			else
				DirectX11RenderAPI::GetDeviceContext()->PSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
		}
		void DirectX11Shader::SetUniformValue(const char* location, const Crow::Math::TVec3<float>& value)
		{
			DX11ConstantBuffer* cb = m_ConstanBuffers[GetLocation(location)];
			DirectX11RenderAPI::GetDeviceContext()->UpdateSubresource(m_ConstanBuffers[GetLocation(location)]->m_ConstantBuffer, 0, NULL, &value, 0, 0);
			if (m_ConstanBuffers[GetLocation(location)]->m_Type == ShaderType::VERTEX)
				DirectX11RenderAPI::GetDeviceContext()->VSSetConstantBuffers(m_ConstanBuffers[GetLocation(location)]->m_Reg, 1, &m_ConstanBuffers[GetLocation(location)]->m_ConstantBuffer);
			else
				DirectX11RenderAPI::GetDeviceContext()->PSSetConstantBuffers(m_ConstanBuffers[GetLocation(location)]->m_Reg, 1, &m_ConstanBuffers[GetLocation(location)]->m_ConstantBuffer);
		}
		void DirectX11Shader::SetUniformValue(const char* location, const Crow::Math::TVec4<float>& value)
		{
			DX11ConstantBuffer* cb = m_ConstanBuffers[GetLocation(location)];
			DirectX11RenderAPI::GetDeviceContext()->UpdateSubresource(m_ConstanBuffers[GetLocation(location)]->m_ConstantBuffer, 0, NULL, &value, 0, 0);
			if (m_ConstanBuffers[GetLocation(location)]->m_Type == ShaderType::VERTEX)
				DirectX11RenderAPI::GetDeviceContext()->VSSetConstantBuffers(m_ConstanBuffers[GetLocation(location)]->m_Reg, 1, &m_ConstanBuffers[GetLocation(location)]->m_ConstantBuffer);
			else
				DirectX11RenderAPI::GetDeviceContext()->PSSetConstantBuffers(m_ConstanBuffers[GetLocation(location)]->m_Reg, 1, &m_ConstanBuffers[GetLocation(location)]->m_ConstantBuffer);
		}
		void DirectX11Shader::SetUniformValue(const char* location, const Math::Mat2& value)
		{
			DX11ConstantBuffer* cb = m_ConstanBuffers[GetLocation(location)];
			DirectX11RenderAPI::GetDeviceContext()->UpdateSubresource(cb->m_ConstantBuffer, 0, NULL, &value, 0, 0);
			if (cb->m_Type == ShaderType::VERTEX)
				DirectX11RenderAPI::GetDeviceContext()->VSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
			else
				DirectX11RenderAPI::GetDeviceContext()->PSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
		}
		void DirectX11Shader::SetUniformValue(const char* location, const Math::Mat3& value)
		{
			DX11ConstantBuffer* cb = m_ConstanBuffers[GetLocation(location)];
			DirectX11RenderAPI::GetDeviceContext()->UpdateSubresource(cb->m_ConstantBuffer, 0, NULL, &value, 0, 0);
			if (cb->m_Type == ShaderType::VERTEX)
				DirectX11RenderAPI::GetDeviceContext()->VSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
			else
				DirectX11RenderAPI::GetDeviceContext()->PSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
		}

		void DirectX11Shader::SetUniformValue(const char* location, const Math::Mat4& value)
		{
			DX11ConstantBuffer* cb = m_ConstanBuffers[GetLocation(location)];
			DirectX11RenderAPI::GetDeviceContext()->UpdateSubresource(cb->m_ConstantBuffer, 0, NULL, &value, 0, 0);
			if (cb->m_Type == ShaderType::VERTEX)
				DirectX11RenderAPI::GetDeviceContext()->VSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
			else
				DirectX11RenderAPI::GetDeviceContext()->PSSetConstantBuffers(cb->m_Reg, 1, &cb->m_ConstantBuffer);
		}

		void DirectX11Shader::SetUniformStruct(const char* location, void* data)
		{
		}

		DXGI_FORMAT DirectX11Shader::ConvertToDXGIFormat(int componentCount)
		{
			switch (componentCount) // Not in sequential order because they are sorted in a way that should return the most common one first
			{
				case 3:
					return DXGI_FORMAT_R32G32B32_FLOAT;
				case 2:
					return DXGI_FORMAT_R32G32_FLOAT;
				case 4:
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
				case 1:
					return DXGI_FORMAT_R32_FLOAT;
				default:
					return DXGI_FORMAT_UNKNOWN;
			}
		}

		DirectX11Shader::UniformType DirectX11Shader::StringToUniformType(std::string& string)
		{
			if (string == "int32")     return UniformType::INT;
			if (string == "float")     return UniformType::FLOAT;
			if (string == "float2")    return UniformType::FLOAT2;
			if (string == "float3")    return UniformType::FLOAT3;
			if (string == "float4")    return UniformType::FLOAT4;
			if (string == "float3x3")  return UniformType::MAT3;
			if (string == "float4x4")  return UniformType::MAT4;
			return UniformType::UNKNOWN;
		}

		uint DirectX11Shader::UniformTypeToSize(UniformType type)
		{
			switch (type)
			{
				case UniformType::INT:      return 4;
				case UniformType::FLOAT:    return 4;
				case UniformType::FLOAT2:   return 4 * 2;
				case UniformType::FLOAT3:   return 4 * 3;
				case UniformType::FLOAT4:
				case UniformType::MAT2:		return 2 * 2 * 4;
				case UniformType::MAT3:     return 4 * 3 * 3;
				case UniformType::MAT4:     return 4 * 4 * 4;
				default: return -1;
			}
		}
	}
}