#pragma once
#include "Crow/Common.h"
#include "Crow/Graphics/Renderer/Buffer.h"

#include "DirectX12RenderAPI.h"


namespace Crow {
	namespace Platform {

		class DirectX12VertexBuffer : public VertexBuffer {
		private:
			ID3D12Resource* m_VertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		public:
			DirectX12VertexBuffer(float* vertices, const uint size, const BufferProperties& prop);
			virtual ~DirectX12VertexBuffer() override;

			static inline std::shared_ptr<VertexBuffer> CreateDirectX12VertexBuffer(float* vertices, const uint size, const BufferProperties& prop) { return  std::make_shared<DirectX12VertexBuffer>(vertices, size, prop); }
			
			virtual void SetBuffer() override;

			const BufferProperties& GetBufferProperties() const override { return m_BufferProperties; }

			virtual void Bind() const override;
			virtual void Unbind() const override;
		private:

			const BufferProperties m_BufferProperties;
			const uint m_Size;
		};

		class DirectX12IndexBuffer : public IndexBuffer {
		private:
			ID3D12Resource* m_IndexBuffer;
			D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		public:
			DirectX12IndexBuffer(ulong* indices, const uint count);
			~DirectX12IndexBuffer() override;

			static inline std::shared_ptr<IndexBuffer> CreateDirectX12IndexBuffer(ulong* indices, const uint count) { return std::make_shared<DirectX12IndexBuffer>(indices, count); }

			virtual void SetBuffer() override;

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual inline const uint GetCount() const override { return m_Count; }
		private:
			const uint m_Count;

			const int m_Size;
		};
	}
}