#include "ArrayBuffer.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	ArrayBuffer* ArrayBuffer::Create(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer)
	{
		return Platform::PlatformAPI::CreateArrayBuffer(vBuffer, iBuffer);
	}
}