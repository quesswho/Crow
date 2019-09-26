#pragma once
#include "Crow/Common.h"
#include "Crow/Graphics/Renderer/Buffer.h"

#include "DirectXRenderAPI.h"

#include <DirectXMath.h>

namespace Crow {
	namespace Platform {

		class DirectXVertexBuffer : public VertexBuffer {
		private:
			ID3D12Resource* m_VertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		public:
			DirectXVertexBuffer(float* vertices, const uint size, const BufferProperties& prop);
			virtual ~DirectXVertexBuffer() override;

			static inline std::shared_ptr<VertexBuffer> CreateDirectXVertexBuffer(float* vertices, const uint size, const BufferProperties& prop) { return  std::make_shared<DirectXVertexBuffer>(vertices, size, prop); }
			
			virtual void SetBuffer() override;

			const BufferProperties& GetBufferProperties() const override { return m_BufferProperties; }

			virtual void Bind() const override;
			virtual void Unbind() const override;
		private:

			const BufferProperties m_BufferProperties;
			const uint m_Size;
		};

		class DirectXIndexBuffer : public IndexBuffer {
		private:
			ID3D12Resource* m_IndexBuffer;
			D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		public:
			DirectXIndexBuffer(ulong* indices, const uint count);
			~DirectXIndexBuffer() override;

			static inline std::shared_ptr<IndexBuffer> CreateDirectXIndexBuffer(ulong* indices, const uint count) { return std::make_shared<DirectXIndexBuffer>(indices, count); }

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