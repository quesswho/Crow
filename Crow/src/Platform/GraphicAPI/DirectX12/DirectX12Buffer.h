#pragma once
#include "Crow/Common.h"

#include "dx12.h"

#include "Crow/Graphics/Renderer/BufferProp.h"

namespace Crow {
	namespace Platform {

		class DirectX12VertexBuffer {
		private:
			ID3D12Resource* m_VertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		public:
			DirectX12VertexBuffer(float* vertices, const uint size, const BufferProperties& prop);
			~DirectX12VertexBuffer();

			static inline std::shared_ptr<DirectX12VertexBuffer> Create(float* vertices, const uint size, const BufferProperties& prop) { return  std::make_shared<DirectX12VertexBuffer>(vertices, size, prop); }
			
			void SetBuffer();

			const BufferProperties& GetBufferProperties() const { return m_BufferProperties; }

			void Bind() const;
			void Unbind() const;
		private:
			const BufferProperties m_BufferProperties;
			const uint m_Size;
		};

		class DirectX12IndexBuffer {
		private:
			ID3D12Resource* m_IndexBuffer;
			D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		public:
			DirectX12IndexBuffer(ulong* indices, const uint count);
			~DirectX12IndexBuffer();

			static inline std::shared_ptr<DirectX12IndexBuffer> Create(ulong* indices, const uint count) { return std::make_shared<DirectX12IndexBuffer>(indices, count); }

			void SetBuffer();

			void Bind() const;
			void Unbind() const;

			inline const uint GetCount() const { return m_Count; }
		private:
			const uint m_Count;
			const int m_Size;
		};
	}
}