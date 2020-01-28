#include "DirectX11RenderAPI.h"

namespace Crow {
	namespace Platform {

		ID3D11Device* DirectX11RenderAPI::s_Device;
		ID3D11DeviceContext* DirectX11RenderAPI::s_DeviceContext;
		IDXGISwapChain* DirectX11RenderAPI::s_SwapChain;
		ID3D11RenderTargetView* DirectX11RenderAPI::s_RenderTargetView;

		ID3D11RasterizerState* DirectX11RenderAPI::s_RasterState;
		ID3D11BlendState* DirectX11RenderAPI::s_BlendState;
		ID3D11DepthStencilView* DirectX11RenderAPI::s_DepthStencilView;
		ID3D11Texture2D* DirectX11RenderAPI::s_DepthStencilBuffer;
		D3D11_VIEWPORT DirectX11RenderAPI::s_ViewPort;

		float* DirectX11RenderAPI::s_ClearColor;

		std::string DirectX11RenderAPI::s_CardName;

		D3D11_CLEAR_FLAG DirectX11RenderAPI::s_DepthMode;
		D3D11_CLEAR_FLAG DirectX11RenderAPI::s_StencilMode;

		bool DirectX11RenderAPI::s_EnableBlending;

		DirectX11RenderAPI::DirectX11RenderAPI()
		{
			m_ShaderFactory = new DirectX11ShaderFactory();
			s_ClearColor = new float[3];
			s_EnableBlending = false;
		}

		DirectX11RenderAPI::~DirectX11RenderAPI()
		{
			s_SwapChain->SetFullscreenState(false, NULL);

			s_Device->Release();
			s_DeviceContext->Release();
			s_SwapChain->Release();

			s_DepthStencilView->Release();
			s_DepthStencilBuffer->Release();
		}

		bool DirectX11RenderAPI::InitAPI(const WindowProperties& winprop, void* hWnd) const
		{
			HRESULT hr;

			IDXGIFactory4* dxgiFactory;
			hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
			if (hr < 0)
			{
				CR_CORE_FATAL("Failed to create DXGIFactory!");
				return false;
			}

			IDXGIAdapter1* adapter;

			int adapterIndex = 0;
			bool adapterFound = false;

			// Next adapter until found
			while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					adapterIndex++; // Discard software devices
					continue;
				}

				hr = D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &s_Device, NULL, &s_DeviceContext);
				if (hr >= 0) // Check if adapter can be used
				{
					adapterFound = true;

					// Get card name
					char adapterdesc[128];
					char aaa = ' ';
					WideCharToMultiByte(CP_ACP, 0, desc.Description, -1, adapterdesc, 128, &aaa, NULL);

					s_CardName = std::string(adapterdesc);

					break;
				}

				adapterIndex++;
			}

			if (!adapterFound)
			{
				CR_CORE_ERROR("No graphics card found that supports Directx 12!");
				return false;
			}

			DXGI_MODE_DESC bufferDesc;

			ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

			bufferDesc.Width = winprop.m_Width;
			bufferDesc.Height = winprop.m_Height;
			bufferDesc.RefreshRate.Numerator = 0;	// TODO: Match refresh rate with every monitor
			bufferDesc.RefreshRate.Denominator = 1;
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			DXGI_SWAP_CHAIN_DESC swapChainDesc;

			ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

			swapChainDesc.BufferDesc = bufferDesc;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		    swapChainDesc.OutputWindow = (HWND)hWnd;
			swapChainDesc.Windowed = true;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			hr = D3D11CreateDeviceAndSwapChain(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &s_SwapChain, &s_Device, NULL, &s_DeviceContext);
			if (hr < 0)
			{
				CR_CORE_FATAL("Failed to initialize DirectX 11!");
				return false;
			}

			ID3D11Texture2D* backBuffer;
			hr = s_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)& backBuffer);
			if (hr < 0)
			{
				CR_CORE_FATAL("Failed to get back buffer!");
				return false;
			}

			hr = s_Device->CreateRenderTargetView(backBuffer, NULL, &s_RenderTargetView);
			backBuffer->Release();
			if (hr < 0)
			{
				CR_CORE_FATAL("Failed to create Render target view!");
				return false;
			}


			s_DeviceContext->OMSetRenderTargets(1, &s_RenderTargetView, NULL);

			D3D11_TEXTURE2D_DESC depthStencilDesc;

			depthStencilDesc.Width = winprop.m_Width;
			depthStencilDesc.Height = winprop.m_Height;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0;
			depthStencilDesc.MiscFlags = 0;

			s_Device->CreateTexture2D(&depthStencilDesc, NULL, &s_DepthStencilBuffer);
			if(s_DepthStencilBuffer > 0)
				s_Device->CreateDepthStencilView(s_DepthStencilBuffer, NULL, &s_DepthStencilView);

			D3D11_RASTERIZER_DESC rasterDesc;

			rasterDesc.AntialiasedLineEnable = false;
			rasterDesc.CullMode = D3D11_CULL_BACK;
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = true;
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.FrontCounterClockwise = true;
			rasterDesc.MultisampleEnable = false;
			rasterDesc.ScissorEnable = false;
			rasterDesc.SlopeScaledDepthBias = 0.0f;

			s_Device->CreateRasterizerState(&rasterDesc, &s_RasterState);
			s_DeviceContext->RSSetState(s_RasterState);

			s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			SetViewPort(winprop.m_Width, winprop.m_Height);

			return true;
		}

		void DirectX11RenderAPI::EndInit() const
		{

			D3D11_RENDER_TARGET_BLEND_DESC rtbd;
			ZeroMemory(&rtbd, sizeof(rtbd));

			rtbd.BlendEnable = s_EnableBlending;
			rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			rtbd.BlendOp = D3D11_BLEND_OP_ADD;
			rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
			rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
			rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			D3D11_BLEND_DESC blendDesc;
			ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

			blendDesc.AlphaToCoverageEnable = false;
			blendDesc.RenderTarget[0] = rtbd;

			s_Device->CreateBlendState(&blendDesc, &s_BlendState);

			s_DeviceContext->OMSetBlendState(s_BlendState, 0, 0xffffffff);
		}

		void DirectX11RenderAPI::Begin() const
		{
			s_DeviceContext->ClearRenderTargetView(s_RenderTargetView, s_ClearColor);
			s_DeviceContext->ClearDepthStencilView(s_DepthStencilView, s_DepthMode | s_StencilMode, 1.0f, 0);
			s_DeviceContext->RSSetViewports(1, &s_ViewPort);
		}

		void DirectX11RenderAPI::End() const
		{
			s_SwapChain->Present(0, 0);
		}

		void DirectX11RenderAPI::ClearColor(float r, float g, float b) const
		{
			s_ClearColor[0] = r;
			s_ClearColor[1] = g;
			s_ClearColor[2] = b;
			s_ClearColor[3] = 1.0f;
		}

		void DirectX11RenderAPI::SetViewPort(uint width, uint height) const
		{
			s_ViewPort.Width = (float)width;
			s_ViewPort.Height = (float)height;
			s_ViewPort.TopLeftX = 0;
			s_ViewPort.TopLeftY = 0;
			s_ViewPort.MinDepth = 0.0f;
			s_ViewPort.MaxDepth = 1.0f;
		}

		void DirectX11RenderAPI::DrawIndices(uint count) const
		{
			s_DeviceContext->DrawIndexedInstanced(count, 1, 0, 0, 0);
		}

		void DirectX11RenderAPI::EnableBlending() const
		{
			s_EnableBlending = true;
		}

		void DirectX11RenderAPI::EnableDepthTest() const
		{
			s_DepthMode = (D3D11_CLEAR_DEPTH);
		}

		void DirectX11RenderAPI::EnableStencilTest() const
		{
			s_StencilMode = (D3D11_CLEAR_STENCIL);
		}

		std::string DirectX11RenderAPI::GetGraphicsInfo() const
		{
			return s_CardName;
		}
	}
}