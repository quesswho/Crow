#pragma once
#include "Crow/Graphics/Renderer/ArrayBuffer.h"

namespace Crow {
	namespace Platform {
		class DirectXArrayBuffer : public ArrayBuffer {
		public:
			DirectXArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer);
			virtual ~DirectXArrayBuffer() override;

			static inline ArrayBuffer* CreateDirectXArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer) { return new DirectXArrayBuffer(vBuffer, iBuffer); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual const inline uint GetCount() const override { return m_IndexBuffer->GetCount(); }
		private:
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
		};
	}
}