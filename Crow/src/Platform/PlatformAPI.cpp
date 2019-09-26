#include "PlatformAPI.h"

namespace Crow {
	namespace Platform {

		GraphicAPI PlatformAPI::s_GraphicAPI;
		ApplicationAPI PlatformAPI::s_ApplicationAPI;

		void PlatformAPI::GraphicAPIInit(GraphicAPI grahpicApi)
		{
			switch (grahpicApi)
			{
				case OPENGL:
					s_GraphicAPI = OPENGL;
					OpenGLInit();
					break;
				case DIRECTX:
					s_GraphicAPI = DIRECTX;
					CheckWindowsError();
					DirectXInit();
					break;
			}
		}

		void PlatformAPI::ApplicationAPIInit(ApplicationAPI appApi)
		{
			switch(appApi)
			{
			case GLFW:
				s_ApplicationAPI = GLFW;
				GLFWInit();
				break;
			case WINDOWS:
				CR_WINDOWSERROR();
				s_ApplicationAPI = WINDOWS;
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

		void PlatformAPI::DirectXInit()
		{
			CreateRenderAPI = &DirectXRenderAPI::CreateDirectXRenderAPI;

			CreateVertexBuffer = &DirectXVertexBuffer::CreateDirectXVertexBuffer;
			CreateIndexBuffer = &DirectXIndexBuffer::CreateDirectXIndexBuffer;

			CreateArrayBuffer = &DirectXArrayBuffer::CreateDirectXArrayBuffer;

			CreateShaderFromPath = &DirectXShader::CreateDirectXShaderFromPath;
			CreateShaderFromSource = &DirectXShader::CreateDirectXShaderFromSource;

			CreateTexture = &OpenGLTexture::CreateOpenGLTexture;
		}

		void PlatformAPI::CheckWindowsError()
		{
			if (s_ApplicationAPI != ApplicationAPI::WINDOWS)
			{
				CR_CORE_WARNING("Cannot use a non Windows application for DirectX!");
			}
		}

	}
}