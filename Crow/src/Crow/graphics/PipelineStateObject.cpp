#include "PipelineStateObject.h"

#include "Platform/PlatformAPI.h"

namespace Crow {

	PipelineStateObject* PipelineStateObject::Create(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Shader* shader)
	{
		return Platform::PlatformAPI::CreatePipelineStateObject(vBuffer, iBuffer, shader);
	}
}