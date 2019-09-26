#include "DirectXPipelineStateObject.h"

#include <glad/glad.h>

namespace Crow {
	namespace Platform {

		DirectXPipelineStateObject::DirectXPipelineStateObject(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Shader* shader)
			: m_Shader((DirectXShader*)shader), m_VertexBuffer(vBuffer), m_IndexBuffer(iBuffer), m_RootSignature(0), m_PSO(0)
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

			if (vBuffer->GetBufferProperties().m_Stride > 0)
			{
				BufferProperties prop = vBuffer->GetBufferProperties();
				const int size = prop.m_Elements.size();
				D3D12_INPUT_ELEMENT_DESC* inputLayout = new D3D12_INPUT_ELEMENT_DESC[size];

				for (int i = 0; i < size; i++)
				{
					BufferElement element = prop.m_Elements[i];
					inputLayout[i] = { element.GetName(), 0, ConvertToDXGIFormat(element.GetComponentCount()), 0, (uint) element.GetOffset() * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				}

				D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
				inputLayoutDesc.NumElements = size;
				inputLayoutDesc.pInputElementDescs = inputLayout;

				D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
				psoDesc.InputLayout = inputLayoutDesc;
				psoDesc.pRootSignature = m_RootSignature;
				psoDesc.VS = m_Shader->GetVertexShader();
				psoDesc.PS = m_Shader->GetFragmentShader();
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
				DirectXRenderAPI::SetPipe(m_PSO);
			}
			else {
				CR_CORE_ERROR("Vertex buffer is missing Properties!");
			}
		}

		DirectXPipelineStateObject::~DirectXPipelineStateObject()
		{
			m_PSO->Release();
			m_RootSignature->Release();
		}

		void DirectXPipelineStateObject::Bind() const
		{
			DirectXRenderAPI::GetCommandList()->SetGraphicsRootSignature(m_RootSignature);
			DirectXRenderAPI::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_VertexBuffer->Bind();
			m_IndexBuffer->Bind();
		}

		void DirectXPipelineStateObject::Unbind() const
		{}

		DXGI_FORMAT DirectXPipelineStateObject::ConvertToDXGIFormat(int componentCount)
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