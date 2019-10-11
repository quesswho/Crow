#include "DirectXShader.h"
#include "Crow/FileUtils.h"

#include <glad/glad.h>
#include <sstream>
#include <regex>

namespace Crow {
	namespace Platform {

		DirectXShader::DirectXShader(const char* name, const char* path, const BufferProperties& shaderInput)
			: m_Name(name), m_ShaderInput(shaderInput), m_ConstantBufferShaderType(0)
		{
			Init(FileUtils::ReadFile(path));
		}

		DirectXShader::DirectXShader(const char* name, std::string& source, const BufferProperties& shaderInput)
			: m_Name(name), m_ShaderInput(shaderInput), m_ConstantBufferShaderType(0)
		{
			Init(source);
		}

		DirectXShader::~DirectXShader()
		{
			m_FragmentShader->Release();
			m_VertexShader->Release();
			m_RootSignature->Release();
			m_PSO->Release();

			for (auto buffer : m_SingleUniformConstantBuffers)
				delete buffer;
		}


		void DirectXShader::Init(std::string& fileSource)
		{
			ShaderType type = UKNOWN;

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
					if(line == "\"") continue;							// Get rid of quotation mark in beginning to allow highlightning
				}

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

			FindConstantBuffer(stringVertexSource);
			CompileShader(stringVertexSource.c_str(), stringFragmentSource.c_str());
			InitPSO();
			DirectXRenderAPI::MapUniform(this);
		}


		void DirectXShader::ReloadFromPath(const char* path)
		{
			Init(FileUtils::ReadFile(path));
		}

		void DirectXShader::ReloadFromSource(std::string& source)
		{
			Init(source);
		}

		void DirectXShader::FindConstantBuffer(std::string& vertex)
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
					reg = line.find(':'); // Using reg as a temporary value before setting register value
					cbuffername = line.substr(strlen("cbuffer") + 1, reg - strlen("cbuffer") - 2);
					if (reg != std::string::npos)
					{
						reg = line.find("register(b", reg);
						if (reg != std::string::npos)
						{
							reg = std::stoi(line.substr((reg + 10), line.size() - reg - 11)); // Convert to int between "register(b" and ')'
							size = 0;
						}
					}

					std::unordered_map<UniformType, std::string> uniforms;

					//DirectXConstantBuffer* buffer = new DirectXConstantBuffer(name, reg);
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
					if (uniforms.size() == 1)
					{
						m_AllocatedSingleUniformCbuffers.emplace(cbuffername, m_AllocatedSingleUniformCbuffers.size());
						m_AllocateSingleUniformCBuffers.emplace(reg, size);
						switch (type)
						{
						case DirectXShader::UniformType::INT:
							m_SingleUniformConstantBuffers.push_back(new DXSingleFieldCBuffer<int>(0, reg));
							break;
						case DirectXShader::UniformType::FLOAT:
							m_SingleUniformConstantBuffers.push_back(new DXSingleFieldCBuffer<float>(0.0f, reg));
							break;
						case DirectXShader::UniformType::FLOAT2:
							m_SingleUniformConstantBuffers.push_back(new DXSingleFieldCBuffer<glm::vec2>(glm::vec2(0.0f), reg));
							break;
						case DirectXShader::UniformType::FLOAT3:
							m_SingleUniformConstantBuffers.push_back(new DXSingleFieldCBuffer<glm::vec3>(glm::vec3(0.0f), reg));
							break;
						case DirectXShader::UniformType::FLOAT4:
							m_SingleUniformConstantBuffers.push_back(new DXSingleFieldCBuffer<glm::vec4>(glm::vec4(0.0f), reg));
							break;
						case DirectXShader::UniformType::MAT3:
							m_SingleUniformConstantBuffers.push_back(new DXSingleFieldCBuffer<glm::mat3x3>(glm::mat3x3(0.0f), reg));
							break;
						case DirectXShader::UniformType::MAT4:
							m_SingleUniformConstantBuffers.push_back(new DXSingleFieldCBuffer<glm::mat4x4>(glm::mat4x4(0.0f), reg));
							break;
						}

					}

					// Performance
					if (m_ConstantBufferShaderType == 2)
						m_ConstantBufferShaderType = 3;
					else if (m_ConstantBufferShaderType == 0)
						m_ConstantBufferShaderType = 1;

					uniforms.clear();
				}
			}
		}


		void DirectXShader::CompileShader(const char* vertex, const char* fragment)
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
				"main",
				"vs_5_0", // Vertex Shader Model 5.0
				compileFlags,
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
				"ps_5_0", // Fragment Shader Model 5.0
				compileFlags,
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

		void DirectXShader::CreateConstantBuffers(int frame)
		{
			const UINT cbvSrvDescriptorSize = DirectXRenderAPI::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvCpuHandle(DirectXRenderAPI::s_MainDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
			CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvGpuHandle(DirectXRenderAPI::s_MainDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
			HRESULT hr;
			for (int i = 0; i < m_AllocatedSingleUniformCbuffers.size(); i++)
			{
				hr = DirectXRenderAPI::GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64),	// Temporary
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_SingleUniformConstantBuffers[i]->m_ConstantBufferUploadHeap));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to create Constant Buffer Upload Heap");
				}
				m_SingleUniformConstantBuffers[i]->m_ConstantBufferUploadHeap->SetName(L"Crow: Constant Buffer Upload Heap");

				cbvSrvCpuHandle.Offset(cbvSrvDescriptorSize);
				cbvSrvGpuHandle.Offset(cbvSrvDescriptorSize);

				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
				cbvDesc.BufferLocation = m_SingleUniformConstantBuffers[i]->m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
				cbvDesc.SizeInBytes = (m_SingleUniformConstantBuffers[i]->GetSize() + 255) & ~255;    // CB size is required to be 256-byte aligned.
				DirectXRenderAPI::GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvCpuHandle);

				m_SingleUniformConstantBuffers[i]->m_ConstantBufferHandle = cbvSrvGpuHandle;

				CD3DX12_RANGE readRange(0, 0);
				hr = m_SingleUniformConstantBuffers[i]->m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_SingleUniformConstantBuffers[i]->m_GPUAddress));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to map constant buffer!");
				}
			}
		}

		void DirectXShader::InitPSO()
		{
			HRESULT hr;

			const int cbCount = m_SingleUniformConstantBuffers.size();

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;

			if (cbCount != 0)
			{
				D3D12_DESCRIPTOR_RANGE* descriptorTableRanges = new D3D12_DESCRIPTOR_RANGE[cbCount];
				for (int i = 0; i < cbCount; i++)
				{
					descriptorTableRanges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
					descriptorTableRanges[i].NumDescriptors = 1;
					descriptorTableRanges[i].BaseShaderRegister = m_SingleUniformConstantBuffers[i]->GetReg();	// Register value E.g. "b2"
					descriptorTableRanges[i].RegisterSpace = 0;
					descriptorTableRanges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
				}

				D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
				descriptorTable.NumDescriptorRanges = cbCount;
				descriptorTable.pDescriptorRanges = &descriptorTableRanges[0];

				D3D12_ROOT_PARAMETER rootParameter[1];
				rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				rootParameter[0].DescriptorTable = descriptorTable;
				rootParameter[0].ShaderVisibility = m_ConstantBufferShaderType < 3 ?
					(m_ConstantBufferShaderType == 1 ? D3D12_SHADER_VISIBILITY_VERTEX : D3D12_SHADER_VISIBILITY_PIXEL)
					: D3D12_SHADER_VISIBILITY_ALL;
				D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
					D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // Only the input assembler stage needs access to the constant buffer.
					D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
					D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;

				rootSignatureDesc.Init(_countof(rootParameter), rootParameter, 0, nullptr, rootSignatureFlags);
			}

			ID3DBlob* signature;
			hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
			if (hr < 0)
			{
				CR_CORE_ERROR("Failed to serialize root signature!");
				return;
			}
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
				psoDesc.DepthStencilState = DirectXRenderAPI::GetDepthStencilDescription();

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

			for(IDXSingleFieldCBuffer* buffer : m_SingleUniformConstantBuffers)
				DirectXRenderAPI::GetCommandList()->SetGraphicsRootDescriptorTable(buffer->GetReg(), buffer->m_ConstantBufferHandle);

		}

		void DirectXShader::Unbind() const
		{}


		int DirectXShader::GetLocation(const char* location)
		{
#ifdef CR_RELEASE
			return m_AllocatedSingleUniformCbuffers.at(location);
#elif CR_DEBUG
			if (m_AllocatedSingleUniformCbuffers.find(location) != m_AllocatedSingleUniformCbuffers.end())
				return m_AllocatedSingleUniformCbuffers.at(location);
			else
			{
				CR_CORE_WARNING("\"{}\" Uniform not Found!", location);
				return 0;
			}
#endif
		}

		void DirectXShader::SetUniformValue(const char* location, int value)
		{
			memcpy(m_SingleUniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}

		void DirectXShader::SetUniformValue(const char* location, float value)
		{
			memcpy(m_SingleUniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}
		void DirectXShader::SetUniformValue(const char* location, glm::vec2& value)
		{
			memcpy(m_SingleUniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}
		void DirectXShader::SetUniformValue(const char* location, glm::vec3& value)
		{
			memcpy(m_SingleUniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}
		void DirectXShader::SetUniformValue(const char* location, glm::vec4& value)
		{
			memcpy(m_SingleUniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}

		void DirectXShader::SetUniformValue(const char* location, const glm::mat3x3& value)
		{
			memcpy(m_SingleUniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}

		void DirectXShader::SetUniformValue(const char* location, const glm::mat4x4& value)
		{
			memcpy(m_SingleUniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
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

		DirectXShader::UniformType DirectXShader::StringToUniformType(std::string& string)
		{
			if (string == "int32")     return DirectXShader::UniformType::INT;
			if (string == "float")     return DirectXShader::UniformType::FLOAT;
			if (string == "float2")    return DirectXShader::UniformType::FLOAT2;
			if (string == "float3")    return DirectXShader::UniformType::FLOAT3;
			if (string == "float4")    return DirectXShader::UniformType::FLOAT4;
			if (string == "float3x3")  return DirectXShader::UniformType::MAT3;
			if (string == "float4x4")  return DirectXShader::UniformType::MAT4;
			return UNKNOWN;
		}

		uint DirectXShader::UniformTypeToSize(DirectXShader::UniformType type)
		{
			switch (type)
			{
				case DirectXShader::UniformType::INT:     return 4;
				case DirectXShader::UniformType::FLOAT:   return 4;
				case DirectXShader::UniformType::FLOAT2:  return 4 * 2;
				case DirectXShader::UniformType::FLOAT3:  return 4 * 3;
				case DirectXShader::UniformType::FLOAT4:  return 4 * 4;
				case DirectXShader::UniformType::MAT3:    return 4 * 3 * 3;
				case DirectXShader::UniformType::MAT4:    return 4 * 4 * 4;
				default: return UNKNOWN;
			}
		}
	}
}