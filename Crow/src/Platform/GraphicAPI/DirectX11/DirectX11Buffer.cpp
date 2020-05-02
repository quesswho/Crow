#include "DirectX11Buffer.h"

#include "DirectX11RenderAPI.h"

namespace Crow {
	namespace Platform {


		DirectX11VertexBuffer::DirectX11VertexBuffer(float* vertices, const uint size, const BufferProperties& prop)
			: m_BufferProperties(prop), m_Size(size), m_StrideInBytes(prop.m_Stride * 4), m_Offset(0)
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));

			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.ByteWidth = size;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA vertexData = {};
			ZeroMemory(&vertexData, sizeof(vertexData));

			vertexData.pSysMem = reinterpret_cast<BYTE*>(vertices);
			DirectX11RenderAPI::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer);

			DirectX11RenderAPI::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_StrideInBytes, &m_Offset);
		}

		DirectX11VertexBuffer::~DirectX11VertexBuffer()
		{
			m_VertexBuffer->Release();
		}

		void DirectX11VertexBuffer::Bind() const
		{
			DirectX11RenderAPI::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_StrideInBytes, &m_Offset);
		}

		void DirectX11VertexBuffer::Unbind() const
		{}

		/////////////

		DirectX11IndexBuffer::DirectX11IndexBuffer(ulong* indices, const uint count)
			: m_Count(count), m_Size(count * sizeof(uint))
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));

			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.ByteWidth = m_Size;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA indexData;

			indexData.pSysMem = reinterpret_cast<BYTE*>(indices);
			DirectX11RenderAPI::GetDevice()->CreateBuffer(&bufferDesc, &indexData, &m_IndexBuffer);

			DirectX11RenderAPI::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		}

		DirectX11IndexBuffer::~DirectX11IndexBuffer()
		{
			m_IndexBuffer->Release();
		}

		void DirectX11IndexBuffer::Bind() const
		{
			DirectX11RenderAPI::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		}

		void DirectX11IndexBuffer::Unbind() const
		{}

	}
}