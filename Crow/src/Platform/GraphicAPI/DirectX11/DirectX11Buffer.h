#pragma once
#include "dx11.h"

#include "Crow/Graphics/Renderer/BufferProp.h"

namespace Crow {
	namespace Platform {

		class DirectX11VertexBuffer {
		private:
			ID3D11Buffer* m_VertexBuffer;
		public:
			DirectX11VertexBuffer(float* vertices, const uint size, const BufferProperties& prop);
			~DirectX11VertexBuffer();

			static inline std::shared_ptr<DirectX11VertexBuffer> Create(float* vertices, const uint size, const BufferProperties& prop) { return  std::make_shared<DirectX11VertexBuffer>(vertices, size, prop); }

			const BufferProperties& GetBufferProperties() const { return m_BufferProperties; }

			void SetBuffer() {}

			void Bind() const;
			void Unbind() const;
		private:
			const BufferProperties m_BufferProperties;
			const uint m_Size;
			const uint m_StrideInBytes;
			const uint m_Offset;
		};

		class DirectX11IndexBuffer {
		private:
			ID3D11Buffer* m_IndexBuffer;
		public:
			DirectX11IndexBuffer(ulong* indices, const uint count);
			~DirectX11IndexBuffer();

			static inline std::shared_ptr<DirectX11IndexBuffer> Create(ulong* indices, const uint count) { return std::make_shared<DirectX11IndexBuffer>(indices, count); }

			void SetBuffer() {}

			void Bind() const;
			void Unbind() const;

			inline const uint GetCount() const { return m_Count; }
		private:
			const uint m_Count;
			const int m_Size;
		};
	}
}