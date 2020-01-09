#include "DirectX11ArrayBuffer.h"

namespace Crow {
	namespace Platform {

		DirectX11ArrayBuffer::DirectX11ArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer)
			: m_VertexBuffer(vBuffer), m_IndexBuffer(iBuffer)
		{}

		DirectX11ArrayBuffer::~DirectX11ArrayBuffer()
		{}

		void DirectX11ArrayBuffer::Bind() const
		{

			m_VertexBuffer->Bind();
			m_IndexBuffer->Bind();
		}

		void DirectX11ArrayBuffer::Unbind() const
		{}

	}
}