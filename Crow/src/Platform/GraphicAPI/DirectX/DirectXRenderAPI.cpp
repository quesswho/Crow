#include "DirectXRenderAPI.h"

namespace Crow {
	namespace Platform {

		ID3D12Device* DirectXRenderAPI::s_Device;
		IDXGISwapChain3* DirectXRenderAPI::s_SwapChain;
		ID3D12CommandQueue* DirectXRenderAPI::s_CommandQueue;
		DXGI_SAMPLE_DESC DirectXRenderAPI::s_SampleDescription;
		ID3D12DescriptorHeap* DirectXRenderAPI::s_rtvDescriptorHeap;
		ID3D12Resource* DirectXRenderAPI::s_RenderTargets[3];
		ID3D12CommandAllocator* DirectXRenderAPI::s_CommandAllocator[3 * 1];
		ID3D12GraphicsCommandList* DirectXRenderAPI::s_CommandList;
		ID3D12Fence* DirectXRenderAPI::s_Fences[3 * 1];

		HANDLE DirectXRenderAPI::s_FenceEvent;
		UINT64 DirectXRenderAPI::s_FenceValue[3 * 1];
		int DirectXRenderAPI::s_Frame;
		int DirectXRenderAPI::s_rtvDescriptorSize;

		std::vector<ID3D12PipelineState*> DirectXRenderAPI::m_PSOs;

		float* DirectXRenderAPI::m_ClearColor;
		D3D12_VIEWPORT DirectXRenderAPI::s_ViewPort;
		D3D12_RECT DirectXRenderAPI::s_ScissorRect;

		std::string DirectXRenderAPI::m_CardName;

		std::vector<VertexBuffer*> DirectXRenderAPI::m_VertexBuffers;
		std::vector<IndexBuffer*> DirectXRenderAPI::m_IndexBuffers;

		DirectXRenderAPI::DirectXRenderAPI()
		{
			m_ClearColor = new float[3];
			m_ShaderFactory = new DirectXShaderFactory();
		}

		DirectXRenderAPI::~DirectXRenderAPI() 
		{
			CloseHandle(s_FenceEvent);
			for (int i = 0; i < 3; ++i)
			{
				s_Frame = i;
				WaitForLastFrame();
			}

			// Get swapchain out of full screen before exiting
			BOOL fs = false;
			if (s_SwapChain->GetFullscreenState(&fs, NULL) >= 0)
				s_SwapChain->SetFullscreenState(false, NULL);

			s_Device->Release();
			s_SwapChain->Release();
			s_CommandQueue->Release();
			s_rtvDescriptorHeap->Release();
			s_CommandList->Release();

			for (int i = 0; i < 3; ++i)
			{
				s_RenderTargets[i]->Release();
				s_CommandAllocator[i]->Release();
				s_Fences[i]->Release();
			};
		}

		void DirectXRenderAPI::CreateDeviceContext() 
		{

		}

		bool DirectXRenderAPI::InitAPI(const WindowProperties& winprop, void* hWnd) const
		{

			const int frameBufferCount = 3;
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

				// feature_level_11 = directx 12
				hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
				if (hr >= 0) // Check if adapter can be used
				{
					adapterFound = true;
					break;
				}

				adapterIndex++;
			}

			if (!adapterFound)
			{
				CR_CORE_ERROR("Graphic card does not support Directx12!");
				return false;
			}
			
			// Create device with previously selected adapter 
			hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&s_Device));
			if (hr < 0)
			{
				CR_CORE_FATAL("Failed to initialize Directx 12!");
				return false;
			}

			auto adapterdescW = DXGI_ADAPTER_DESC1();
			adapter->GetDesc1(&adapterdescW);
			
			char adapterdesc[128];
			char aaa = ' ';
			WideCharToMultiByte(CP_ACP, 0, adapterdescW.Description, -1, adapterdesc, 128, &aaa, NULL);

			m_CardName = std::string(adapterdesc);


			D3D12_COMMAND_QUEUE_DESC cqDesc = {};
			cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;


			hr = s_Device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&s_CommandQueue));
			if (hr < 0)
			{
				CR_CORE_FATAL("Failed to create Command Queue!");
				return false;
			}

			DXGI_MODE_DESC backBufferDesc = {};
			backBufferDesc.Width = winprop.m_Width;
			backBufferDesc.Height = winprop.m_Height;
			backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA 8 bit for each channel

			s_SampleDescription = {};
			s_SampleDescription.Count = 1; // TODO: Add Multisampling 

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount = frameBufferCount;
			swapChainDesc.BufferDesc = backBufferDesc;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Render to screeen
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.OutputWindow = (HWND)hWnd;
			swapChainDesc.SampleDesc = s_SampleDescription;
			swapChainDesc.Windowed = true; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

			IDXGISwapChain* tempSwapChain;

			dxgiFactory->CreateSwapChain(
				s_CommandQueue,
				&swapChainDesc,
				&tempSwapChain
			);

			s_SwapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);

			s_Frame = s_SwapChain->GetCurrentBackBufferIndex();

			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = frameBufferCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			hr = s_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&s_rtvDescriptorHeap));
			if (hr < 0)
			{
				CR_CORE_FATAL("Failed to create DescriptorHeap!");
				return false;
			}

			s_rtvDescriptorSize = s_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(s_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

			// Create a RTV for each buffer
			for (int i = 0; i < frameBufferCount; i++)
			{
				hr = s_SwapChain->GetBuffer(i, IID_PPV_ARGS(&s_RenderTargets[i]));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to RTV for buffer {}", i);
					return false;
				}

				s_Device->CreateRenderTargetView(s_RenderTargets[i], nullptr, rtvHandle);

				rtvHandle.Offset(1, s_rtvDescriptorSize);
			}

			// Create command allocator

			for (int i = 0; i < frameBufferCount; i++)
			{
				hr = s_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&s_CommandAllocator[i]));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to CreateCommand for buffer {}", i);
					return false;
				}
			}

			// create the command list with the first allocator
			hr = s_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, s_CommandAllocator[0], NULL, IID_PPV_ARGS(&s_CommandList));
			if (hr < 0)
			{
				CR_CORE_ERROR("Failed to Create CommandList!");
				return false;
			}

			for (int i = 0; i < frameBufferCount; i++)
			{
				hr = s_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&s_Fences[i]));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to Create fence for buffer {}", i);
					return false;
				}
				s_FenceValue[i] = 0; // set the initial fence value to 0
			}

			// create a handle to a fence event
			s_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (s_FenceEvent == nullptr)
			{
				CR_CORE_ERROR("Failed to create handle for fence event!");
				return false;
			}

			SetViewPort(winprop.m_Width, winprop.m_Height);

			return true;
		}

		void DirectXRenderAPI::Begin() const
		{
			WaitForLastFrame();

			s_CommandAllocator[s_Frame]->Reset();

			for(auto pipe : m_PSOs)
				s_CommandList->Reset(s_CommandAllocator[s_Frame], pipe);

			s_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(s_RenderTargets[s_Frame], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(s_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), s_Frame, s_rtvDescriptorSize);
			s_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
			s_CommandList->ClearRenderTargetView(rtvHandle, m_ClearColor, 0, nullptr);
			s_CommandList->RSSetViewports(1, &s_ViewPort);
			s_CommandList->RSSetScissorRects(1, &s_ScissorRect);
		}

		void DirectXRenderAPI::End() const
		{
			s_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(s_RenderTargets[s_Frame], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
			s_CommandList->Close();

			ID3D12CommandList* commandLists[] = { s_CommandList };

			s_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

			s_CommandQueue->Signal(s_Fences[s_Frame], s_FenceValue[s_Frame]);

			s_SwapChain->Present(0, 0);
		}

		void DirectXRenderAPI::ClearColor(float r, float g, float b) const
		{
			m_ClearColor[0] = r;
			m_ClearColor[1] = g;
			m_ClearColor[2] = b;
			m_ClearColor[3] = 1.0f;
		}

		void DirectXRenderAPI::SetViewPort(uint width, uint height) const
		{
			s_ViewPort.Width = width;
			s_ViewPort.Height = height;
			s_ViewPort.TopLeftX = 0;
			s_ViewPort.TopLeftY = 0;
			s_ViewPort.MinDepth = 0.0f;
			s_ViewPort.MaxDepth = 1.0f;

			s_ScissorRect.bottom = height;
			s_ScissorRect.right = width;
			s_ScissorRect.top = 0;
			s_ScissorRect.left = 0;
		}

		void DirectXRenderAPI::DrawIndices(uint count) const
		{
			s_CommandList->DrawIndexedInstanced(count, 1, 0, 0, 0);
		}

		void DirectXRenderAPI::EnableAlpha() const
		{

		}

		void DirectXRenderAPI::EnableDepthTest() const
		{

		}

		std::string DirectXRenderAPI::GetGraphicsInfo() const
		{
			return m_CardName;
		}

		void DirectXRenderAPI::WaitForLastFrame()
		{
			s_Frame = s_SwapChain->GetCurrentBackBufferIndex();

			// if Frame is not done
			if (s_Fences[s_Frame]->GetCompletedValue() < s_FenceValue[s_Frame])
			{
				s_Fences[s_Frame]->SetEventOnCompletion(s_FenceValue[s_Frame], s_FenceEvent);

				WaitForSingleObject(s_FenceEvent, INFINITE);
			}
			s_FenceValue[s_Frame]++;
		}

		void DirectXRenderAPI::EndInit() const
		{
			s_CommandList->Close();
			ID3D12CommandList* commandLists[] = { s_CommandList };
			s_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

			s_FenceValue[s_Frame]++;
			s_CommandQueue->Signal(s_Fences[s_Frame], s_FenceValue[s_Frame]);

			for (VertexBuffer* vBuffer : m_VertexBuffers)
			{
				vBuffer->SetBuffer();
			}

			for (IndexBuffer* iBuffer : m_IndexBuffers)
			{
				iBuffer->SetBuffer();
			}
		}
	}
}