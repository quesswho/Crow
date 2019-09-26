#pragma once
#include "Crow/Graphics/ArrayBuffer.h"

#include "DirectXRenderAPI.h"
#include "DirectXShader.h"

namespace Crow {
	namespace Platform {
		class DirectXArrayBuffer : public ArrayBuffer {
		private:
			ID3D12PipelineState* m_ArrayBuffer;
			ID3D12RootSignature* m_RootSignature;
		public:
			DirectXArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer);
			virtual ~DirectXArrayBuffer() override;

			static inline ArrayBuffer* CreateDirectXPipelineStateObject(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer) { return new DirectXArrayBuffer(vBuffer, iBuffer); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual const inline uint GetCount() const override { return m_IndexBuffer->GetCount(); }
		private:
			DXGI_FORMAT ConvertToDXGIFormat(int componentCount);
		private:
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
		};
	}
}