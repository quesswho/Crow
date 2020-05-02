#pragma once
#include "Platform/GraphicAPI/DirectX11/DirectX11Buffer.h"

namespace Crow {
	namespace Platform {
		class DirectX11ArrayBuffer {
		public:
			DirectX11ArrayBuffer(const std::shared_ptr<DirectX11VertexBuffer> vBuffer, const std::shared_ptr<DirectX11IndexBuffer> iBuffer);
			~DirectX11ArrayBuffer();

			static inline DirectX11ArrayBuffer* Create(const std::shared_ptr<DirectX11VertexBuffer> vBuffer, const std::shared_ptr<DirectX11IndexBuffer> iBuffer) { return new DirectX11ArrayBuffer(vBuffer, iBuffer); }

			void Bind() const;
			void Unbind() const;

			const inline uint GetCount() const { return m_IndexBuffer->GetCount(); }
		private:
			std::shared_ptr<DirectX11IndexBuffer> m_IndexBuffer;
			std::shared_ptr<DirectX11VertexBuffer> m_VertexBuffer;
		};
	}
}