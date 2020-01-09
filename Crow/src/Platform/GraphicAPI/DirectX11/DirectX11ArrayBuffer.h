#pragma once
#include "Crow/Graphics/Renderer/ArrayBuffer.h"

namespace Crow {
	namespace Platform {
		class DirectX11ArrayBuffer : public ArrayBuffer {
		public:
			DirectX11ArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer);
			virtual ~DirectX11ArrayBuffer() override;

			static inline ArrayBuffer* CreateDirectX11ArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer) { return new DirectX11ArrayBuffer(vBuffer, iBuffer); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual const inline uint GetCount() const override { return m_IndexBuffer->GetCount(); }
		private:
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
		};
	}
}