#pragma once

#include "Renderer/Buffer.h"
#include "Shader.h"

namespace Crow {

	class PipelineStateObject {
	public:
		virtual ~PipelineStateObject() = default;

		static PipelineStateObject* Create(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Shader* shader);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual Shader* GetShader() const = 0;

		virtual const inline uint GetCount() const = 0;
	};
}