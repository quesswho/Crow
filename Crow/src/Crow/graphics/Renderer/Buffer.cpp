#include "Buffer.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, const uint size, const BufferProperties& prop)
	{ 
		return Platform::PlatformAPI::CreateVertexBuffer(vertices, size, prop);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(ulong* indices, const uint count)
	{
		return Platform::PlatformAPI::CreateIndexBuffer(indices, count);
	}

}
