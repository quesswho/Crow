#pragma once

#include "Crow/Graphics/PipelineStateObject.h"

#include "OpenGLBuffer.h"
#include "OpenGLShader.h"

namespace Crow {
	namespace Platform {

		class OpenGLPipelineStateObject : public PipelineStateObject {
		private:
			uint m_VertexArrayID;
		public:
			OpenGLPipelineStateObject(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Shader* shader);
			virtual ~OpenGLPipelineStateObject() override;

			static inline PipelineStateObject* CreateOpenGLPipelineStateObject(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Shader* shader) { return new OpenGLPipelineStateObject(vBuffer, iBuffer, shader); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual inline Shader* GetShader() const override { return m_Shader; }
			virtual const inline uint GetCount() const override { return m_IndexBuffer->GetCount(); }
		private:
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
			Shader* m_Shader;
		};
	}
}