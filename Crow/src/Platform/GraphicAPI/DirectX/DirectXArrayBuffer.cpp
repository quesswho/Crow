#include "DirectXArrayBuffer.h"

#include <glad/glad.h>

namespace Crow {
	namespace Platform {

		DirectXArrayBuffer::DirectXArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer)
			: m_VertexBuffer(vBuffer), m_IndexBuffer(iBuffer)
		{}

		DirectXArrayBuffer::~DirectXArrayBuffer()
		{}

		void DirectXArrayBuffer::Bind() const
		{
			
			m_VertexBuffer->Bind();
			m_IndexBuffer->Bind();
		}

		void DirectXArrayBuffer::Unbind() const
		{}

	}
}