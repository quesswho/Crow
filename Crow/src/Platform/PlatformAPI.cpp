#include "PlatformAPI.h"

//#include "Crow/Graphics/Renderer/RenderAPI.h"

namespace Crow {
	namespace Platform {

		RenderAPI PlatformAPI::s_RenderAPI;

		void PlatformAPI::Init(RenderAPI api)
		{
			switch (api)
			{
				case OPENGL:
					s_RenderAPI = OPENGL;
					OpenGLInit();
					break;
			}
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
	}
}