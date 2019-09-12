#pragma once
#include "Crow/Common.h"
#include "Crow/Graphics/Renderer/Buffer.h"

#include <initializer_list>
#include <vector>

namespace Crow {
	namespace Platform {

		class OpenGLVertexBuffer : public VertexBuffer {
		private:
			uint m_BufferID;
		public:
			OpenGLVertexBuffer(const float* vertices, const uint size, const BufferProperties& prop);
			~OpenGLVertexBuffer() override;

			static inline std::shared_ptr<VertexBuffer> CreateOpenGLVertexBuffer(const float* vertices, const uint size, const BufferProperties& prop) { return  std::make_shared<OpenGLVertexBuffer>(vertices, size, prop); }

			const BufferProperties& GetBufferProperties() const override { return m_BufferProperties; }

			virtual void Bind() const override;
			virtual void Unbind() const override;
		private:
			const BufferProperties m_BufferProperties;
		};

		class OpenGLIndexBuffer : public IndexBuffer {
		private:
			uint m_BufferID;

		public:
			OpenGLIndexBuffer(const uint* indices, const uint count);
			~OpenGLIndexBuffer() override;

			static inline std::shared_ptr<IndexBuffer> CreateOpenGLIndexBuffer(const uint* indices, const uint count) { return std::make_shared<OpenGLIndexBuffer>(indices, count); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual inline const uint GetCount() const override { return m_Count; }
		private:
			const uint m_Count;

		};
	}
}