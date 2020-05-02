#include "DirectX12Buffer.h"

#include "DirectX12RenderAPI.h"
namespace Crow {
	namespace Platform {


		DirectX12VertexBuffer::DirectX12VertexBuffer(float* vertices, const uint size, const BufferProperties& prop)
			: m_BufferProperties(prop), m_Size(size)
		{
			m_VertexBufferView = {};

			DirectX12RenderAPI::GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(size),
				D3D12_RESOURCE_STATE_COPY_DEST, // Ready to copy
				nullptr,
				IID_PPV_ARGS(&m_VertexBuffer));

			m_VertexBuffer->SetName(L"Crow: Vertex Buffer resource heap"); // Debug name

			ID3D12Resource* vertexBufferUploadHeap;
			DirectX12RenderAPI::GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(m_Size),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&vertexBufferUploadHeap));

			m_VertexBuffer->SetName(L"Crow: Vertex Buffer Upload resource heap");

			D3D12_SUBRESOURCE_DATA vertexData = {};

			vertexData.pData = reinterpret_cast<BYTE*>(vertices);
			vertexData.RowPitch = m_Size;
			vertexData.SlicePitch = m_Size;

			UpdateSubresources(DirectX12RenderAPI::GetCommandList(), m_VertexBuffer, vertexBufferUploadHeap, 0, 0, 1, &vertexData);
			DirectX12RenderAPI::GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

			DirectX12RenderAPI::Upload(this);
		}

		DirectX12VertexBuffer::~DirectX12VertexBuffer()
		{
			m_VertexBuffer->Release();
		}

		void DirectX12VertexBuffer::SetBuffer()
		{
			m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
			m_VertexBufferView.StrideInBytes = m_BufferProperties.m_Stride * 4;
			m_VertexBufferView.SizeInBytes = m_Size;
		}

		void DirectX12VertexBuffer::Bind() const
		{
			DirectX12RenderAPI::GetCommandList()->IASetVertexBuffers(0, 1, &m_VertexBufferView);
		}

		void DirectX12VertexBuffer::Unbind() const
		{}

		/////////////

		DirectX12IndexBuffer::DirectX12IndexBuffer(ulong* indices, const uint count)
			: m_Count(count), m_Size(count * sizeof(uint))
		{
			m_IndexBufferView = {};

			DirectX12RenderAPI::GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(m_Size),
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&m_IndexBuffer));

			m_IndexBuffer->SetName(L"Crow: Index Buffer resource heap");

			ID3D12Resource* indexBufferUploadHeap;
			DirectX12RenderAPI::GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(m_Size),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&indexBufferUploadHeap));

			m_IndexBuffer->SetName(L"Crow: Index Buffer Upload resource heap");

			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = reinterpret_cast<BYTE*>(indices);
			indexData.RowPitch = m_Size;
			indexData.SlicePitch = m_Size;

			UpdateSubresources(DirectX12RenderAPI::GetCommandList(), m_IndexBuffer, indexBufferUploadHeap, 0, 0, 1, &indexData);
			DirectX12RenderAPI::GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

			DirectX12RenderAPI::Upload(this);
		}

		DirectX12IndexBuffer::~DirectX12IndexBuffer()
		{
			m_IndexBuffer->Release();
		}

		void DirectX12IndexBuffer::SetBuffer()
		{
			m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
			m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
			m_IndexBufferView.SizeInBytes = m_Size;
		}

		void DirectX12IndexBuffer::Bind() const
		{
			DirectX12RenderAPI::GetCommandList()->IASetIndexBuffer(&m_IndexBufferView);
		}

		void DirectX12IndexBuffer::Unbind() const
		{}

	}
}