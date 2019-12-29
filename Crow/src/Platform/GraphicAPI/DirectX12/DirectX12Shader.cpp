#include "DirectX12Shader.h"
#include "Crow/FileUtils.h"

#include <glad/glad.h>
#include <sstream>

namespace Crow {
	namespace Platform {

		DirectX12Shader::DirectX12Shader(const char* name, const char* path, const BufferProperties& shaderInput)
			: m_Name(name), m_ShaderInput(shaderInput), m_ConstantBufferShaderType(0)
		{
			Init(FileUtils::ReadFile(path));
		}

		DirectX12Shader::DirectX12Shader(const char* name, std::string& source, const BufferProperties& shaderInput)
			: m_Name(name), m_ShaderInput(shaderInput), m_ConstantBufferShaderType(0)
		{
			Init(source);
		}

		DirectX12Shader::~DirectX12Shader()
		{
			m_FragmentShader->Release();
			m_VertexShader->Release();
			m_RootSignature->Release();
			m_PSO->Release();

			for (auto buffer : m_UniformConstantBuffers)
				delete buffer;
		}


		void DirectX12Shader::Init(std::string& fileSource)
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
					if(line == "\"") continue;							// Get rid of quotation mark in beginning to allow highlightning
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
			InitPSO();
			DirectX12RenderAPI::MapUniform(this);
		}


		void DirectX12Shader::ReloadFromPath(const char* path)
		{
			Init(FileUtils::ReadFile(path));
		}

		void DirectX12Shader::ReloadFromSource(std::string& source)
		{
			Init(source);
		}

		void DirectX12Shader::FindConstantBuffer(std::string& vertex, std::string& fragment)
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

					m_ConstantBufferLocations.emplace(cbuffername, m_ConstantBufferLocations.size());
					m_UniformConstantBuffers.push_back(new DX12ConstantBuffer(size, reg));


					// Performance
					if (m_ConstantBufferShaderType == 2)
						m_ConstantBufferShaderType = 3;
					else if (m_ConstantBufferShaderType == 0)
						m_ConstantBufferShaderType = 1;


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

					m_ConstantBufferLocations.emplace(cbuffername, m_ConstantBufferLocations.size());
					m_UniformConstantBuffers.push_back(new DX12ConstantBuffer(size, reg));


					// Performance
					if (m_ConstantBufferShaderType == 1)
						m_ConstantBufferShaderType = 3;
					else if (m_ConstantBufferShaderType == 0)
						m_ConstantBufferShaderType = 2;

					uniforms.clear();
				}
			}
		}


		void DirectX12Shader::CompileShader(const char* vertex, const char* fragment)
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
				"PSmain",
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

		void DirectX12Shader::CreateConstantBuffers()
		{
			const UINT cbvSrvDescriptorSize = DirectX12RenderAPI::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvCpuHandle(DirectX12RenderAPI::s_MainDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
			CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvGpuHandle(DirectX12RenderAPI::s_MainDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
			HRESULT hr;
			for (int i = 0; i < m_ConstantBufferLocations.size(); i++)
			{
				hr = DirectX12RenderAPI::GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64),	// Temporary
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_UniformConstantBuffers[i]->m_ConstantBufferUploadHeap));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to create Constant Buffer Upload Heap");
				}
				m_UniformConstantBuffers[i]->m_ConstantBufferUploadHeap->SetName(L"Crow: Constant Buffer Upload Heap");

				cbvSrvCpuHandle.Offset(cbvSrvDescriptorSize);
				cbvSrvGpuHandle.Offset(cbvSrvDescriptorSize);

				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
				cbvDesc.BufferLocation = m_UniformConstantBuffers[i]->m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
				cbvDesc.SizeInBytes = (m_UniformConstantBuffers[i]->m_Size + 255) & ~255;    // CB size is required to be 256-byte aligned.
				DirectX12RenderAPI::GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvCpuHandle);

				m_UniformConstantBuffers[i]->m_ConstantBufferHandle = cbvSrvGpuHandle;

				CD3DX12_RANGE readRange(0, 0);
				hr = m_UniformConstantBuffers[i]->m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_UniformConstantBuffers[i]->m_GPUAddress));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to map constant buffer!");
				}
			}
		}

		void DirectX12Shader::InitPSO()
		{
			HRESULT hr;

			const uint cbCount = (uint)m_UniformConstantBuffers.size();

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;

			if (cbCount != 0)
			{
				D3D12_DESCRIPTOR_RANGE* descriptorTableRanges = new D3D12_DESCRIPTOR_RANGE[cbCount];
				for (uint i = 0; i < cbCount; i++)
				{
					descriptorTableRanges[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
					descriptorTableRanges[i].NumDescriptors = 1;
					descriptorTableRanges[i].BaseShaderRegister = m_UniformConstantBuffers[i]->m_Reg;	// Register value E.g. "b2"
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
			else {
				rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			}

			ID3DBlob* signature;
			hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
			if (hr < 0)
			{
				CR_CORE_ERROR("Failed to serialize root signature!");
				return;
			}
			hr = DirectX12RenderAPI::GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
			if (hr < 0)
			{
				CR_CORE_ERROR("Failed to create root signature!");
				return;
			}

			if (m_ShaderInput.m_Stride > 0)
			{
				const uint size = (uint)m_ShaderInput.m_Elements.size();
				D3D12_INPUT_ELEMENT_DESC* inputLayout = new D3D12_INPUT_ELEMENT_DESC[size];

				for (uint i = 0; i < size; i++)
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
				psoDesc.SampleDesc = DirectX12RenderAPI::GetSampleDescription();
				psoDesc.SampleMask = 0xffffffff;
				psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
				psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
				psoDesc.NumRenderTargets = 1;
				psoDesc.DepthStencilState = DirectX12RenderAPI::GetDepthStencilDescription();

				hr = DirectX12RenderAPI::GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to create graphics pipeline state!");
				}
				DirectX12RenderAPI::AddPipeline(m_PSO);
			}
			else {
				CR_CORE_ERROR("Vertex buffer is missing Properties!");
			}
		}

		void DirectX12Shader::Bind() const
		{
			DirectX12RenderAPI::GetCommandList()->SetGraphicsRootSignature(m_RootSignature);
			DirectX12RenderAPI::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			for(DX12ConstantBuffer* buffer : m_UniformConstantBuffers)
				DirectX12RenderAPI::GetCommandList()->SetGraphicsRootDescriptorTable(buffer->m_Reg, buffer->m_ConstantBufferHandle);

		}

		void DirectX12Shader::Unbind() const
		{}


		int DirectX12Shader::GetLocation(const char* location)
		{
#ifdef CR_RELEASE
			return m_ConstantBufferLocations.at(location);
#elif CR_DEBUG
			if (m_ConstantBufferLocations.find(location) != m_ConstantBufferLocations.end())
				return m_ConstantBufferLocations.at(location);
			else
			{
				CR_CORE_WARNING("\"{}\" Uniform not found!", location);
				return 0;
			}
#endif
		}

		void DirectX12Shader::SetUniformValue(const char* location, int value)
		{
			memcpy(m_UniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}

		void DirectX12Shader::SetUniformValue(const char* location, float value)
		{
			memcpy(m_UniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}
		void DirectX12Shader::SetUniformValue(const char* location, const Crow::Math::Vec2<float>& value)
		{
			memcpy(m_UniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}
		void DirectX12Shader::SetUniformValue(const char* location, const Crow::Math::Vec3<float>& value)
		{
			memcpy(m_UniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}
		void DirectX12Shader::SetUniformValue(const char* location, const Crow::Math::Vec4<float>& value)
		{
			memcpy(m_UniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}
		void DirectX12Shader::SetUniformValue(const char* location, const glm::mat2x2& value)
		{
			memcpy(m_UniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}
		void DirectX12Shader::SetUniformValue(const char* location, const Math::Mat3<float>& value)
		{
			memcpy(m_UniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}

		void DirectX12Shader::SetUniformValue(const char* location, const Math::Mat4<float>& value)
		{
			memcpy(m_UniformConstantBuffers[GetLocation(location)]->m_GPUAddress, &value, sizeof(value));
		}

		void DirectX12Shader::SetUniformStruct(const char* location, void* data)
		{
			DX12ConstantBuffer* cb = m_UniformConstantBuffers[GetLocation(location)];
			memcpy(cb->m_GPUAddress, &data, cb->m_Size);
		}

		DXGI_FORMAT DirectX12Shader::ConvertToDXGIFormat(int componentCount)
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

		DirectX12Shader::UniformType DirectX12Shader::StringToUniformType(std::string& string)
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

		uint DirectX12Shader::UniformTypeToSize(UniformType type)
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