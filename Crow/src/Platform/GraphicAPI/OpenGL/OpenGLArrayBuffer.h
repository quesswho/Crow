#pragma once
#include "OpenGLBuffer.h"

namespace Crow {
	namespace Platform {

		class OpenGLArrayBuffer {
		private:
			uint m_VertexArrayID;
		public:
			OpenGLArrayBuffer(const std::shared_ptr<OpenGLVertexBuffer> vBuffer, const std::shared_ptr<OpenGLIndexBuffer> iBuffer);
			~OpenGLArrayBuffer();

			static inline OpenGLArrayBuffer* Create(const std::shared_ptr<OpenGLVertexBuffer> vBuffer, const std::shared_ptr<OpenGLIndexBuffer> iBuffer) { return new OpenGLArrayBuffer(vBuffer, iBuffer); }

			void Bind() const;
			void Unbind() const;

			const inline uint GetCount() const { return m_IndexBuffer->GetCount(); }
		private:
			std::shared_ptr<OpenGLVertexBuffer> m_VertexBuffer;
			std::shared_ptr<OpenGLIndexBuffer> m_IndexBuffer;
		};
	}
}