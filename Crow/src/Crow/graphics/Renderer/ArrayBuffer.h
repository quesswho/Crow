#pragma once
#include "Buffer.h"

#include <memory>

namespace Crow {

	class ArrayBuffer {
	private:
		uint m_VertexArrayID;
	public:
		ArrayBuffer();
		~ArrayBuffer();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const inline uint GetCount() const { return m_IndexBuffer->GetCount(); }

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}