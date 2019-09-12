#pragma once
#include "Crow/Graphics/Renderer/ArrayBuffer.h"

namespace Crow {
	namespace Platform {
		class OpenGLArrayBuffer : public ArrayBuffer {
		private:
			uint m_VertexArrayID;
		public:
			OpenGLArrayBuffer();
			virtual ~OpenGLArrayBuffer() override;

			static inline ArrayBuffer* CreateOpenGLArrayBuffer() { return new OpenGLArrayBuffer(); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
			virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

			virtual const inline uint GetCount() const override { return m_IndexBuffer->GetCount(); }

		private:
			std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
		};
	}
}