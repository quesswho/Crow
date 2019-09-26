#pragma once

#include "Buffer.h"

namespace Crow {

	class ArrayBuffer {
	public:
		virtual ~ArrayBuffer() = default;

		static ArrayBuffer* Create(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const inline uint GetCount() const = 0;
	};
}