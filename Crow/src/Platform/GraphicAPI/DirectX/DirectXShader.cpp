#include "DirectXShader.h"
#include "Crow/FileUtils.h"

#include <glad/glad.h>
#include <sstream>
#include <regex>

namespace Crow {
	namespace Platform {

		DirectXShader::DirectXShader(const char* name, const char* path, const BufferProperties& shaderInput)
			: m_Name(name), m_ShaderInput(shaderInput)
		{
			Init(FileUtils::ReadFile(path));
		}

		DirectXShader::DirectXShader(const char* name, std::string& source, const BufferProperties& shaderInput)
			: m_Name(name), m_ShaderInput(shaderInput)
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

			//Vertex Shader

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
			InitPSO();
		}


		void DirectXShader::ReloadFromPath(const char* path)
		{
			Init(FileUtils::ReadFile(path));
		}

		void DirectXShader::ReloadFromSource(std::string& source)
		{
			Init(source);
		}

		void DirectXShader::InitPSO()
		{
			HRESULT hr;

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ID3DBlob* signature;
			D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
			hr = DirectXRenderAPI::GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));

			if (hr < 0)
			{
				CR_CORE_ERROR("Failed to create root signature!");
				return;
			}

			if (m_ShaderInput.m_Stride > 0)
			{
				const int size = m_ShaderInput.m_Elements.size();
				D3D12_INPUT_ELEMENT_DESC* inputLayout = new D3D12_INPUT_ELEMENT_DESC[size];

				for (int i = 0; i < size; i++)
				{
					BufferElement element = m_ShaderInput.m_Elements[i];
					inputLayout[i] = { element.GetName(), 0, ConvertToDXGIFormat(element.GetComponentCount()), 0, (uint)element.GetOffset() * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				}

				D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
				inputLayoutDesc.NumElements = size;
				inputLayoutDesc.pInputElementDescs = inputLayout;

				D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
				psoDesc.InputLayout = inputLayoutDesc;
				psoDesc.pRootSignature = m_RootSignature;
				psoDesc.VS = m_CompiledVertexShader;
				psoDesc.PS = m_CompiledFragmentShader;
				psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
				psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
				psoDesc.SampleDesc = DirectXRenderAPI::GetSampleDescription();
				psoDesc.SampleMask = 0xffffffff;
				psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
				psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
				psoDesc.NumRenderTargets = 1;

				hr = DirectXRenderAPI::GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to create graphics pipeline state!");
				}
				DirectXRenderAPI::AddPipeline(m_PSO);
			}
			else {
				CR_CORE_ERROR("Vertex buffer is missing Properties!");
			}
		}

		void DirectXShader::Bind() const
		{
			DirectXRenderAPI::GetCommandList()->SetGraphicsRootSignature(m_RootSignature);
			DirectXRenderAPI::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		void DirectXShader::Unbind() const
		{}


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

		DXGI_FORMAT DirectXShader::ConvertToDXGIFormat(int componentCount)
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
	}
}