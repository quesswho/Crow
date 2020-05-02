#pragma once
#include "Crow/Common.h"
#include "Crow/Graphics/Renderer/BufferProp.h"

namespace Crow {
	namespace Platform {

		class OpenGLVertexBuffer {
		private:
			uint m_BufferID;
		public:
			OpenGLVertexBuffer(float* vertices, const uint size, const BufferProperties& prop);
			~OpenGLVertexBuffer();

			static inline std::shared_ptr<OpenGLVertexBuffer> Create(float* vertices, const uint size, const BufferProperties& prop) { return std::make_shared<OpenGLVertexBuffer>(vertices, size, prop); }

			void SetBuffer() {}

			const BufferProperties& GetBufferProperties() const { return m_BufferProperties; }

			void Bind() const;
			void Unbind() const;
		private:
			const BufferProperties m_BufferProperties;
		};

		class OpenGLIndexBuffer {
		private:
			uint m_BufferID;

		public:
			OpenGLIndexBuffer(ulong* indices, const uint count);
			virtual ~OpenGLIndexBuffer();

			static inline std::shared_ptr<OpenGLIndexBuffer> Create(ulong* indices, const uint count) { return std::make_shared<OpenGLIndexBuffer>(indices, count); }

			void SetBuffer() {}

			void Bind() const;
			void Unbind() const;

			inline const uint GetCount() const { return m_Count; }
		private:
			const uint m_Count;

		};
	}
}