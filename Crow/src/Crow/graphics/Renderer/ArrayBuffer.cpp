#include "ArrayBuffer.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	ArrayBuffer* ArrayBuffer::Create()
	{
		return Platform::PlatformAPI::CreateArrayBuffer();
	}
}