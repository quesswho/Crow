#pragma once

#include "Crow/Graphics/Renderer/Buffer.h"

#include "DirectX11RenderAPI.h"

namespace Crow {
	namespace Platform {

		class DirectX11VertexBuffer : public VertexBuffer {
		private:
			ID3D11Buffer* m_VertexBuffer;
		public:
			DirectX11VertexBuffer(float* vertices, const uint size, const BufferProperties& prop);
			virtual ~DirectX11VertexBuffer() override;

			static inline std::shared_ptr<VertexBuffer> CreateDirectX11VertexBuffer(float* vertices, const uint size, const BufferProperties& prop) { return  std::make_shared<DirectX11VertexBuffer>(vertices, size, prop); }

			const BufferProperties& GetBufferProperties() const override { return m_BufferProperties; }

			virtual void SetBuffer() override {}

			virtual void Bind() const override;
			virtual void Unbind() const override;
		private:
			const BufferProperties m_BufferProperties;
			const uint m_Size;
			const uint m_StrideInBytes;
			const uint m_Offset;
		};

		class DirectX11IndexBuffer : public IndexBuffer {
		private:
			ID3D11Buffer* m_IndexBuffer;
		public:
			DirectX11IndexBuffer(ulong* indices, const uint count);
			~DirectX11IndexBuffer() override;

			static inline std::shared_ptr<IndexBuffer> CreateDirectX11IndexBuffer(ulong* indices, const uint count) { return std::make_shared<DirectX11IndexBuffer>(indices, count); }

			virtual void SetBuffer() override {}

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual inline const uint GetCount() const override { return m_Count; }
		private:
			const uint m_Count;
			const int m_Size;
		};
	}
}