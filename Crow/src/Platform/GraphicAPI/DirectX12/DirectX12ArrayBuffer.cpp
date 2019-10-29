#include "DirectX12ArrayBuffer.h"

#include <glad/glad.h>

namespace Crow {
	namespace Platform {

		DirectX12ArrayBuffer::DirectX12ArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer)
			: m_VertexBuffer(vBuffer), m_IndexBuffer(iBuffer)
		{}

		DirectX12ArrayBuffer::~DirectX12ArrayBuffer()
		{}

		void DirectX12ArrayBuffer::Bind() const
		{
			
			m_VertexBuffer->Bind();
			m_IndexBuffer->Bind();
		}

		void DirectX12ArrayBuffer::Unbind() const
		{}

	}
}