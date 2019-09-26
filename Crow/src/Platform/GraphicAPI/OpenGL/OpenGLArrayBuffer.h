#pragma once

#include "Crow/Graphics/Renderer/ArrayBuffer.h"

#include "OpenGLBuffer.h"

namespace Crow {
	namespace Platform {

		class OpenGLArrayBuffer : public ArrayBuffer {
		private:
			uint m_VertexArrayID;
		public:
			OpenGLArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer);
			virtual ~OpenGLArrayBuffer() override;

			static inline ArrayBuffer* CreateOpenGLArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer) { return new OpenGLArrayBuffer(vBuffer, iBuffer); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual const inline uint GetCount() const override { return m_IndexBuffer->GetCount(); }
		private:
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
		};
	}
}