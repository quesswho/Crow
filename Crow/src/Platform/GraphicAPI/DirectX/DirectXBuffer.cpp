#include "DirectXBuffer.h"

namespace Crow {
	namespace Platform {


		DirectXVertexBuffer::DirectXVertexBuffer(float* vertices, const uint size, const BufferProperties& prop)
			: m_BufferProperties(prop), m_Size(size)
		{
			m_VertexBufferView = {};

			DirectXRenderAPI::GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(size),
				D3D12_RESOURCE_STATE_COPY_DEST, // Ready to copy
				nullptr,
				IID_PPV_ARGS(&m_VertexBuffer));

			m_VertexBuffer->SetName(L"Crow: Vertex Buffer resource heap"); // Debug name

			ID3D12Resource* vertexBufferUploadHeap;
			DirectXRenderAPI::GetDevice()->CreateCommittedResource(
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

			UpdateSubresources(DirectXRenderAPI::GetCommandList(), m_VertexBuffer, vertexBufferUploadHeap, 0, 0, 1, &vertexData);
			DirectXRenderAPI::GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

			DirectXRenderAPI::Upload(this);
		}

		DirectXVertexBuffer::~DirectXVertexBuffer()
		{
			m_VertexBuffer->Release();
		}

		void DirectXVertexBuffer::SetBuffer()
		{
			m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
			m_VertexBufferView.StrideInBytes = m_BufferProperties.m_Stride * 4;
			m_VertexBufferView.SizeInBytes = m_Size;
		}

		void DirectXVertexBuffer::Bind() const
		{
			DirectXRenderAPI::GetCommandList()->IASetVertexBuffers(0, 1, &m_VertexBufferView);
		}

		void DirectXVertexBuffer::Unbind() const
		{}

		/////////////

		DirectXIndexBuffer::DirectXIndexBuffer(ulong* indices, const uint count)
			: m_Count(count), m_Size(count * sizeof(uint))
		{
			m_IndexBufferView = {};

			DirectXRenderAPI::GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(m_Size),
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&m_IndexBuffer));

			m_IndexBuffer->SetName(L"Crow: Index Buffer resource heap");

			ID3D12Resource* indexBufferUploadHeap;
			DirectXRenderAPI::GetDevice()->CreateCommittedResource(
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

			UpdateSubresources(DirectXRenderAPI::GetCommandList(), m_IndexBuffer, indexBufferUploadHeap, 0, 0, 1, &indexData);
			DirectXRenderAPI::GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

			DirectXRenderAPI::Upload(this);
		}

		DirectXIndexBuffer::~DirectXIndexBuffer()
		{
			m_IndexBuffer->Release();
		}

		void DirectXIndexBuffer::SetBuffer()
		{
			m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
			m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
			m_IndexBufferView.SizeInBytes = m_Size;
		}

		void DirectXIndexBuffer::Bind() const
		{
			DirectXRenderAPI::GetCommandList()->IASetIndexBuffer(&m_IndexBufferView);
		}

		void DirectXIndexBuffer::Unbind() const
		{}

	}
}