#pragma once

#include "Crow/Graphics/Renderer/RenderAPI.h"
#include "DirectX11ShaderFactory.h"

#include <d3d11.h>

#include <dxgi1_4.h>

namespace Crow {
	namespace Platform {

		class DirectX11RenderAPI : public AbstractRenderAPI {
		public:
			DirectX11RenderAPI();
			virtual ~DirectX11RenderAPI() override;

			static AbstractRenderAPI* CreateDirectX11RenderAPI() { return new DirectX11RenderAPI(); }

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

			virtual inline const char* GetAPIName() const override { return "DirectX 11"; }
			virtual inline ShaderFactory* GetShaderFactory() const override { return m_ShaderFactory; }

			virtual std::string GetGraphicsInfo() const override;

			static inline ID3D11Device* GetDevice() { return s_Device; }
			static inline ID3D11DeviceContext* GetDeviceContext() { return s_DeviceContext; }
		private:
			DirectX11ShaderFactory* m_ShaderFactory;

			static ID3D11Device* s_Device;
			static ID3D11DeviceContext* s_DeviceContext;
			static IDXGISwapChain* s_SwapChain;
			static ID3D11RenderTargetView* s_RenderTargetView;

			static ID3D11RasterizerState* s_RasterState;
			static ID3D11DepthStencilView* s_DepthStencilView;
			static ID3D11Texture2D* s_DepthStencilBuffer;
			static D3D11_VIEWPORT s_ViewPort;

			static float* s_ClearColor; // RGBA

			static std::string s_CardName;

			static D3D11_CLEAR_FLAG m_DepthMode;
			static D3D11_CLEAR_FLAG m_StencilMode;
		};
	}
}