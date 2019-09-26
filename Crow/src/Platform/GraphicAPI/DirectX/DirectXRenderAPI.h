#pragma once

#include "Crow/Graphics/Renderer/RenderAPI.h"

#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>

#include "DirectXShaderFactory.h"
#include "DirectXBuffer.h"

namespace Crow {
	namespace Platform {

		class DirectXRenderAPI : public AbstractRenderAPI {
		public:
			DirectXRenderAPI();
			virtual ~DirectXRenderAPI() override;

			static AbstractRenderAPI* CreateDirectXRenderAPI() { return new DirectXRenderAPI(); }

			virtual void CreateDeviceContext() override;

			virtual bool InitAPI(const WindowProperties& winprop, void* hWnd) const override;
			virtual void EndInit() const override;

			virtual void Begin() const override;
			virtual void End() const override;

			virtual inline void Clear() const override {}
			virtual inline void ClearColor(float r, float g, float b) const override;
			virtual inline void SetViewPort(uint width, uint height) const override;
			virtual inline void DrawIndices(uint count) const override;
			virtual inline void EnableAlpha() const override;
			virtual inline void EnableDepthTest() const override;

			virtual inline const char* GetAPIName() const override { return "DirectX 12"; }
			virtual inline ShaderFactory* GetShaderFactory() const override { return m_ShaderFactory; }

			virtual std::string GetGraphicsInfo() const override;

			static inline ID3D12Device* GetDevice() { return s_Device; }
			static inline ID3D12GraphicsCommandList* GetCommandList() { return s_CommandList; }
			static inline DXGI_SAMPLE_DESC GetSampleDescription() { return s_SampleDescription; }

			static inline D3D12_VIEWPORT GetViewPort() { return s_ViewPort; }
			static inline D3D12_RECT GetScissorRect() { return s_ScissorRect; }

			static void SetPipe(ID3D12PipelineState* pipe) { m_Pipe = pipe; }

			static void Upload(VertexBuffer* vBuffer) { m_VertexBuffers.push_back(vBuffer); }
			static void Upload(IndexBuffer* iBuffer) { m_IndexBuffers.push_back(iBuffer); }
		private:
			static void WaitForLastFrame();
		private:
			static ID3D12PipelineState* m_Pipe;

			DirectXShaderFactory* m_ShaderFactory;

			static ID3D12Device* s_Device;
			static IDXGISwapChain3* s_SwapChain;
			static ID3D12CommandQueue* s_CommandQueue;
			static DXGI_SAMPLE_DESC s_SampleDescription;

			static ID3D12DescriptorHeap* s_rtvDescriptorHeap;

			static ID3D12Resource* s_RenderTargets[3];
			static ID3D12CommandAllocator* s_CommandAllocator[3 * 1]; // Triple buffering multiplied by amount of rendering threads

			static ID3D12GraphicsCommandList* s_CommandList;

			static ID3D12Fence* s_Fences[3 * 1];

			static D3D12_VIEWPORT s_ViewPort;
			static D3D12_RECT s_ScissorRect;

			static HANDLE s_FenceEvent;

			static UINT64 s_FenceValue[3 * 1];

			static int s_Frame;
			static int s_rtvDescriptorSize;

			static float *m_ClearColor; // RGBA

			static std::string m_CardName;

			static std::vector<VertexBuffer*> m_VertexBuffers;
			static std::vector<IndexBuffer*> m_IndexBuffers;
		};
	}
}