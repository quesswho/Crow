#include "PlatformAPI.h"

namespace Crow {
	namespace Platform {

		GraphicAPI PlatformAPI::s_GraphicAPI;
		ApplicationAPI PlatformAPI::s_ApplicationAPI;

		void PlatformAPI::GraphicAPIInit(GraphicAPI grahpicApi)
		{
			switch (grahpicApi)
			{
				case GraphicAPI::OPENGL:
					s_GraphicAPI = GraphicAPI::OPENGL;
					OpenGLInit();
					break;
				case GraphicAPI::DIRECTX12:
					s_GraphicAPI = GraphicAPI::DIRECTX12;
					if (!CheckWindowsError()) GraphicAPIInit(GraphicAPI::OPENGL);
					DirectX12Init();
					break;
			}
		}

		void PlatformAPI::ApplicationAPIInit(ApplicationAPI appApi)
		{
			switch(appApi)
			{
				case ApplicationAPI::GLFW:
					s_ApplicationAPI = ApplicationAPI::GLFW;
					GLFWInit();
					break;
				case ApplicationAPI::WINDOWS:
					CR_WINDOWSERROR();
					s_ApplicationAPI = ApplicationAPI::WINDOWS;
					WindowsInit();
					break;
			}
		}

		void PlatformAPI::GLFWInit()
		{
			CreateWindowAPI = &GLFWAPIWindow::CreateGLFWWindow;
		}

		void PlatformAPI::WindowsInit()
		{
			CreateWindowAPI = &WindowsAPIWindow::CreateWindowsWindow;
		}

		void PlatformAPI::OpenGLInit()
		{
			CreateRenderAPI = &OpenGLRenderAPI::CreateOpenGLRenderAPI;

			CreateVertexBuffer = &OpenGLVertexBuffer::CreateOpenGLVertexBuffer;
			CreateIndexBuffer = &OpenGLIndexBuffer::CreateOpenGLIndexBuffer;

			CreateArrayBuffer = &OpenGLArrayBuffer::CreateOpenGLArrayBuffer;

			CreateShaderFromPath = &OpenGLShader::CreateOpenGLShaderFromPath;
			CreateShaderFromSource = &OpenGLShader::CreateOpenGLShaderFromSource;

			CreateTexture = &OpenGLTexture::CreateOpenGLTexture;
		}

		void PlatformAPI::DirectX12Init()
		{
			CreateRenderAPI = &DirectX12RenderAPI::CreateDirectX12RenderAPI;

			CreateVertexBuffer = &DirectX12VertexBuffer::CreateDirectX12VertexBuffer;
			CreateIndexBuffer = &DirectX12IndexBuffer::CreateDirectX12IndexBuffer;

			CreateArrayBuffer = &DirectX12ArrayBuffer::CreateDirectX12ArrayBuffer;

			CreateShaderFromPath = &DirectX12Shader::CreateDirectX12ShaderFromPath;
			CreateShaderFromSource = &DirectX12Shader::CreateDirectX12ShaderFromSource;

			CreateTexture = &OpenGLTexture::CreateOpenGLTexture;						//TODO: replace with D3D12 class
		}

		bool PlatformAPI::CheckWindowsError()
		{
			if (s_ApplicationAPI != ApplicationAPI::WINDOWS)
			{
				CR_CORE_WARNING("Cannot use a non Windows application for DirectX! Using OpenGL instead.");
				return false;
			}
			return true;
		}

	}
}