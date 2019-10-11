#include "DirectXRenderAPI.h"

namespace Crow {
	namespace Platform {

		ID3D12Device* DirectXRenderAPI::s_Device;
		IDXGISwapChain3* DirectXRenderAPI::s_SwapChain;
		ID3D12CommandQueue* DirectXRenderAPI::s_CommandQueue;
		DXGI_SAMPLE_DESC DirectXRenderAPI::s_SampleDescription;
		ID3D12DescriptorHeap* DirectXRenderAPI::s_rtvDescriptorHeap;
		ID3D12Resource* DirectXRenderAPI::s_RenderTargets[3];
		ID3D12CommandAllocator* DirectXRenderAPI::s_CommandAllocator;
		ID3D12GraphicsCommandList* DirectXRenderAPI::s_CommandList;
		ID3D12Fence* DirectXRenderAPI::s_Fence;

		HANDLE DirectXRenderAPI::s_FenceEvent;
		UINT64 DirectXRenderAPI::s_FenceValue;
		int DirectXRenderAPI::s_Frame;
		int DirectXRenderAPI::s_rtvDescriptorSize;

		std::vector<ID3D12PipelineState*> DirectXRenderAPI::s_PSOs;
		uint DirectXRenderAPI::s_FrameBufferCount;

		float* DirectXRenderAPI::s_ClearColor;
		D3D12_VIEWPORT DirectXRenderAPI::s_ViewPort;
		D3D12_RECT DirectXRenderAPI::s_ScissorRect;

		ID3D12DescriptorHeap* DirectXRenderAPI::s_MainDescriptorHeap;
		D3D12_DEPTH_STENCIL_DESC DirectXRenderAPI::s_DepthStencilDesc;
		ID3D12Resource* DirectXRenderAPI::s_DepthStencilBuffer;
		ID3D12DescriptorHeap* DirectXRenderAPI::s_DepthStencilDescriptorHeap;

		std::string DirectXRenderAPI::s_CardName;

		std::vector<Shader*> DirectXRenderAPI::s_MappingShader;
		std::vector<VertexBuffer*> DirectXRenderAPI::s_VertexBuffers;
		std::vector<IndexBuffer*> DirectXRenderAPI::s_IndexBuffers;

		bool DirectXRenderAPI::s_DepthTest;
		bool DirectXRenderAPI::s_StencilTest;
		bool DirectXRenderAPI::s_Initializing;
		bool DirectXRenderAPI::s_IsPopulating;

		DirectXRenderAPI::DirectXRenderAPI()
		{
			s_FrameBufferCount = 3;
			s_ClearColor = new float[3];
			m_ShaderFactory = new DirectXShaderFactory();
			s_DepthTest = false;
			s_StencilTest = false;
			s_Initializing = false;
			s_IsPopulating = false;
		}

		DirectXRenderAPI::~DirectXRenderAPI() 
		{
			WaitForPreviousFrame();
			CloseHandle(s_FenceEvent);

			// Get swapchain out of full screen before exiting
			BOOL fs = false;
			if (s_SwapChain->GetFullscreenState(&fs, NULL) >= 0)
				s_SwapChain->SetFullscreenState(false, NULL);

			s_Device->Release();
			s_SwapChain->Release();
			s_CommandQueue->Release();
			s_rtvDescriptorHeap->Release();
			s_CommandList->Release();
			s_CommandAllocator->Release();
			s_Fence->Release();

			s_MainDescriptorHeap->Release();

			for (int i = 0; i < 3; ++i)
			{
				s_RenderTargets[i]->Release();
			};

			s_DepthStencilBuffer->Release();
			s_DepthStencilDescriptorHeap->Release();

			delete m_ShaderFactory;
		}

		bool DirectXRenderAPI::InitAPI(const WindowProperties& winprop, void* hWnd) const
		{

			s_Initializing = true;

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
				CR_CORE_ERROR("No graphics card found that supports Directx 12!");
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

			s_CardName = std::string(adapterdesc);


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
			swapChainDesc.BufferCount = s_FrameBufferCount;
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
			rtvHeapDesc.NumDescriptors = s_FrameBufferCount;
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
			for (int i = 0; i < s_FrameBufferCount; i++)
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

			hr = s_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&s_CommandAllocator));
			if (hr < 0)
			{
				CR_CORE_ERROR("Failed to Create CommandAllocator!");
				return false;
			}


			// create the command list with the first allocator
			hr = s_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, s_CommandAllocator, NULL, IID_PPV_ARGS(&s_CommandList));
			if (hr < 0)
			{
				CR_CORE_ERROR("Failed to Create CommandList!");
				return false;
			}

			hr = s_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&s_Fence));
			if (hr < 0)
			{
				CR_CORE_ERROR("Failed to Create fence");
				return false;
			}
			s_FenceValue = 1;

			// create a handle to a fence event
			s_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (s_FenceEvent == nullptr)
			{
				CR_CORE_ERROR("Failed to create handle for fence event!");
				return false;
			}

			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			s_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&s_DepthStencilDescriptorHeap));

			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
			depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CLEAR_VALUE clearValue = {};
			clearValue.Format = DXGI_FORMAT_D32_FLOAT;
			clearValue.DepthStencil.Depth = 1.0f;
			clearValue.DepthStencil.Stencil = 0.0f;

			s_Device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, winprop.m_Width, winprop.m_Height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&clearValue,
				IID_PPV_ARGS(&s_DepthStencilBuffer)
			);
			s_DepthStencilDescriptorHeap->SetName(L"Crow: Depth & Stencil Resource Heap");

			s_Device->CreateDepthStencilView(s_DepthStencilBuffer, &depthStencilDesc, s_DepthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

			SetViewPort(winprop.m_Width, winprop.m_Height);

			return true;
		}

		void DirectXRenderAPI::Begin() const
		{
			WaitForPreviousFrame();

			s_CommandAllocator->Reset();

			for(auto pipe : s_PSOs)
				s_CommandList->Reset(s_CommandAllocator, pipe);

			s_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(s_RenderTargets[s_Frame], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(s_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), s_Frame, s_rtvDescriptorSize);
			CD3DX12_CPU_DESCRIPTOR_HANDLE depthStencilHandle(s_DepthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
			s_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &depthStencilHandle);
			s_CommandList->ClearRenderTargetView(rtvHandle, s_ClearColor, 0, nullptr);
			s_CommandList->ClearDepthStencilView(s_DepthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
			ID3D12DescriptorHeap* descriptorHeaps[] = { s_MainDescriptorHeap };
			s_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

			s_CommandList->RSSetViewports(1, &s_ViewPort);
			s_CommandList->RSSetScissorRects(1, &s_ScissorRect);

			s_IsPopulating = true;
		}

		void DirectXRenderAPI::End() const
		{
			s_IsPopulating = false;

			s_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(s_RenderTargets[s_Frame], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
			s_CommandList->Close();

			ID3D12CommandList* commandLists[] = { s_CommandList };

			s_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

			s_CommandQueue->Signal(s_Fence, s_FenceValue);

			s_SwapChain->Present(0, 0);
		}

		void DirectXRenderAPI::ClearColor(float r, float g, float b) const
		{
			s_ClearColor[0] = r;
			s_ClearColor[1] = g;
			s_ClearColor[2] = b;
			s_ClearColor[3] = 1.0f;
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
			if (s_Initializing)
			{
				s_DepthTest = true;
				UpdateDepthStencilDescription();
			}
			else 
			{
				CR_CORE_WARNING("Cannot enable Depth Test after initialization!");
			}
		}

		std::string DirectXRenderAPI::GetGraphicsInfo() const
		{
			return s_CardName;
		}

		void DirectXRenderAPI::WaitForPreviousFrame()
		{
			s_Frame = s_SwapChain->GetCurrentBackBufferIndex();

			// if Frame is not done
			if (s_Fence->GetCompletedValue() < s_FenceValue)
			{
				s_Fence->SetEventOnCompletion(s_FenceValue, s_FenceEvent);

				WaitForSingleObject(s_FenceEvent, INFINITE);
			}

			s_FenceValue++;
		}

		void DirectXRenderAPI::EndInit() const
		{
			InitConstantBuffers();
			s_CommandList->Close();
			ID3D12CommandList* commandLists[] = { s_CommandList };
			s_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

			s_FenceValue++;
			s_CommandQueue->Signal(s_Fence, s_FenceValue);


			// Set buffer data
			for (VertexBuffer* vBuffer : s_VertexBuffers)
			{
				vBuffer->SetBuffer();
			}

			for (IndexBuffer* iBuffer : s_IndexBuffers)
			{
				iBuffer->SetBuffer();
			}

			s_Initializing = false;
		}

		void DirectXRenderAPI::InitConstantBuffers()
		{
			HRESULT hr;

			int descriptorsCount = 0;
			for (int i = 0; i < s_MappingShader.size(); i++)
				descriptorsCount += static_cast<DirectXShader*>(s_MappingShader[i])->GetCBufferCount();

				D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
				heapDesc.NumDescriptors = descriptorsCount;
				heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				hr = s_Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&s_MainDescriptorHeap));
				if (hr < 0)
				{
					CR_CORE_ERROR("Failed to create Main Descriptor Heap");
				}

				for (Shader* shader : s_MappingShader) // Doing this at the end because thats when all shaders should be created
					shader->CreateConstantBuffers(-1);
		}

		void DirectXRenderAPI::UpdateDepthStencilDescription()
		{

			const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp = // a stencil operation structure, again does not really matter since stencil testing is turned off
			{ 
				D3D12_STENCIL_OP_KEEP, 
				D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP_KEEP,
				D3D12_COMPARISON_FUNC_ALWAYS
			};

			s_DepthStencilDesc.DepthEnable = s_DepthTest;
			s_DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			s_DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			s_DepthStencilDesc.StencilEnable = s_StencilTest;
			s_DepthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
			s_DepthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
			s_DepthStencilDesc.FrontFace = defaultStencilOp;
			s_DepthStencilDesc.BackFace = defaultStencilOp;
		}
	}
}