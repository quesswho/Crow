#pragma once

#include "Crow/Graphics/Renderer/RenderAPI.h"

#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>

#include "DirectX12Shader.h"
#include "DirectX12ShaderFactory.h"
#include "DirectX12Buffer.h"

namespace Crow {
	namespace Platform {

		class DirectX12RenderAPI : public AbstractRenderAPI {
		public:
			DirectX12RenderAPI();
			virtual ~DirectX12RenderAPI() override;

			static AbstractRenderAPI* CreateDirectX12RenderAPI() { return new DirectX12RenderAPI(); }

			virtual bool InitAPI(const WindowProperties& winprop, void* hWnd) const override;
			virtual void EndInit() const override;

			virtual void Begin() const override;
			virtual void End() const override;

			virtual inline void ClearColor(float r, float g, float b) const override;
			virtual inline void SetViewPort(uint width, uint height) const override;
			virtual inline void DrawIndices(uint count) const override;
			virtual inline void EnableAlpha() const override;
			virtual inline void EnableDepthTest() const override;
			virtual inline void EnableStencilTest() const override;

			virtual inline const char* GetAPIName() const override { return "DirectX 12"; }
			virtual inline ShaderFactory* GetShaderFactory() const override { return m_ShaderFactory; }

			virtual std::string GetGraphicsInfo() const override;

			static inline ID3D12Device* GetDevice() { return s_Device; }
			static inline ID3D12GraphicsCommandList* GetCommandList() { return s_CommandList; }
			//static inline ID3D12DescriptorHeap* GetMainDescriptorHeap(int frame) { return s_MainDescriptorHeap[frame]; }
			static inline DXGI_SAMPLE_DESC GetSampleDescription() { return s_SampleDescription; }

			static inline D3D12_VIEWPORT GetViewPort() { return s_ViewPort; }
			static inline D3D12_RECT GetScissorRect() { return s_ScissorRect; }
			static inline D3D12_DEPTH_STENCIL_DESC GetDepthStencilDescription() { return s_DepthStencilDesc; }
			static inline uint GetFrameBufferCount() { return s_FrameBufferCount; }
			static inline int GetFrame() { return s_Frame; }

			static void AddPipeline(ID3D12PipelineState* pipe) { s_PSOs.push_back(pipe); }

			static void MapUniform(Shader* shader) { s_MappingShader.push_back(shader); }
			static void Upload(VertexBuffer* vBuffer) { s_VertexBuffers.push_back(vBuffer); }
			static void Upload(IndexBuffer* iBuffer) { s_IndexBuffers.push_back(iBuffer); }

			static ID3D12DescriptorHeap* s_MainDescriptorHeap;
		private:
			static void WaitForPreviousFrame();
			static void InitConstantBuffers();
			static void UpdateDepthStencilDescription();
		private:
			static std::vector<ID3D12PipelineState*> s_PSOs;
			static const uint s_FrameBufferCount = 2;
			DirectX12ShaderFactory* m_ShaderFactory;

			static ID3D12Device* s_Device;
			static IDXGISwapChain3* s_SwapChain;
			static ID3D12CommandQueue* s_CommandQueue;
			static DXGI_SAMPLE_DESC s_SampleDescription;

			static ID3D12DescriptorHeap* s_rtvDescriptorHeap;

			static ID3D12Resource* s_RenderTargets[s_FrameBufferCount];
			static ID3D12CommandAllocator* s_CommandAllocator;

			static ID3D12GraphicsCommandList* s_CommandList;

			static ID3D12Fence* s_Fence;
			static HANDLE s_FenceEvent;
			static UINT64 s_FenceValue;

			static D3D12_VIEWPORT s_ViewPort;
			static D3D12_RECT s_ScissorRect;

			static D3D12_DEPTH_STENCIL_DESC s_DepthStencilDesc;
			static ID3D12Resource* s_DepthStencilBuffer;
			static ID3D12DescriptorHeap* s_DepthStencilDescriptorHeap;

			static int s_Frame;
			static int s_rtvDescriptorSize;

			static float *s_ClearColor; // RGBA

			static std::string s_CardName;

			static std::vector<Shader*> s_MappingShader;
			static std::vector<VertexBuffer*> s_VertexBuffers;
			static std::vector<IndexBuffer*> s_IndexBuffers;

			static bool s_DepthTest;
			static bool s_StencilTest;
			static bool s_Initializing;
			static bool s_IsPopulating; // If not End() has been called after Begin()
		};
	}
}