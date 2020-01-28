#pragma once

#include "GraphicAPI/OpenGL/OpenGLRenderAPI.h"

// WINDOWS
#include "Application/Windows/WindowsAPIWindow.h"

// GLFW
#include "Application/GLFW/GLFWAPIWindow.h"


// OPENGL
#include "GraphicAPI/OpenGL/OpenGLArrayBuffer.h"
#include "GraphicAPI/OpenGL/OpenGLBuffer.h"
#include "GraphicAPI/OpenGL/OpenGLShader.h"
#include "GraphicAPI/OpenGL/OpenGLTexture.h"
#include "GraphicAPI/OpenGL/OpenGLFont.h"

// DIRECTX 12
#include "GraphicAPI/DirectX12/DirectX12RenderAPI.h"
#include "GraphicAPI/DirectX12/DirectX12ArrayBuffer.h"
#include "GraphicAPI/DirectX12/DirectX12Buffer.h"
#include "GraphicAPI/DirectX12/DirectX12Shader.h"

// DIRECTX 11
#include "GraphicAPI/DirectX11/DirectX11RenderAPI.h"
#include "GraphicAPI/DirectX11/DirectX11Buffer.h"
#include "GraphicAPI/DirectX11/DirectX11ArrayBuffer.h"
#include "GraphicAPI/DirectX11/DirectX11Shader.h"
#include "GraphicAPI/DirectX11/DirectX11Texture.h"
#include "GraphicAPI/DirectX11/DirectX11Font.h"

namespace Crow {
	namespace Platform {

		enum class GraphicAPI {
			OPENGL,
			DIRECTX11,
			DIRECTX12 // WIP don't use
		};

		enum class ApplicationAPI {
			GLFW,
			WINDOWS
		};

		class PlatformAPI {
		public:
			static GraphicAPI s_GraphicAPI;
			static ApplicationAPI s_ApplicationAPI;

			static void GraphicAPIInit(GraphicAPI grahpicApi); // Set all callbacks to right graphics api
			static void ApplicationAPIInit(ApplicationAPI appApi); // Choses which ApplicationAPI will be used

			static inline Window* (*CreateWindowAPI)(WindowProperties props); // Would have wanted to call the function "CreateWindow"

			static inline std::shared_ptr<VertexBuffer> (*CreateVertexBuffer)(float* vertices, const uint size, const BufferProperties& prop);
			static inline std::shared_ptr<IndexBuffer> (*CreateIndexBuffer)(ulong* indices, const uint count);


			static inline ArrayBuffer* (*CreateArrayBuffer)(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer);

			static inline Shader* (*CreateShaderFromPath)(const char* name, const char* path, const BufferProperties& shaderInput);
			static inline Shader* (*CreateShaderFromSource)(const char* name, std::string& source, const BufferProperties& shaderInput);

			static inline Texture* (*CreateTexture)(const char* path, const TextureProperties& props);

			static inline Font* (*CreateTTFont)(const char* path); // Create Truetype Font
			static inline Font* (*CreateTTSFont)(const char* path, Shader* shader); // Create Truetype Font with custom Shader

			static inline AbstractRenderAPI* (*CreateRenderAPI)();

		private:
			// Applications
			static void GLFWInit();
			static void WindowsInit();

			// Graphics APIs
			static void OpenGLInit();
			static void DirectX12Init();
			static void DirectX11Init();

			static bool CheckWindowsError();
		};
	}
}