#pragma once
#include "Crow/Common.h"

#include "DirectX12Buffer.h"

namespace Crow {
	namespace Platform {
		class DirectX12ArrayBuffer {
		public:
			DirectX12ArrayBuffer(const std::shared_ptr<DirectX12VertexBuffer> vBuffer, const std::shared_ptr<DirectX12IndexBuffer> iBuffer);
			~DirectX12ArrayBuffer();

			static inline DirectX12ArrayBuffer* Create(const std::shared_ptr<DirectX12VertexBuffer> vBuffer, const std::shared_ptr<DirectX12IndexBuffer> iBuffer) { return new DirectX12ArrayBuffer(vBuffer, iBuffer); }

			void Bind() const;
			void Unbind() const;

			const inline uint GetCount() const { return m_IndexBuffer->GetCount(); }
		private:
			std::shared_ptr<DirectX12IndexBuffer> m_IndexBuffer;
			std::shared_ptr<DirectX12VertexBuffer> m_VertexBuffer;
		};
	}
}