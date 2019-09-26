#pragma once
#include "Crow/Graphics/PipelineStateObject.h"

#include "DirectXRenderAPI.h"
#include "DirectXShader.h"

namespace Crow {
	namespace Platform {
		class DirectXPipelineStateObject : public PipelineStateObject {
		private:
			ID3D12PipelineState* m_PSO;
			ID3D12RootSignature* m_RootSignature;
		public:
			DirectXPipelineStateObject(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Shader* shader);
			virtual ~DirectXPipelineStateObject() override;

			static inline PipelineStateObject* CreateDirectXPipelineStateObject(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Shader* shader) { return new DirectXPipelineStateObject(vBuffer, iBuffer, shader); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual inline Shader* GetShader() const override { return m_Shader; }
			virtual const inline uint GetCount() const override { return m_IndexBuffer->GetCount(); }
		private:
			DXGI_FORMAT ConvertToDXGIFormat(int componentCount);
		private:
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
			DirectXShader* m_Shader;
		};
	}
}