#pragma once

#include "GraphicAPI/OpenGL/OpenGLRenderAPI.h"

// GLFW
#include "Application/GLFW/GLFWAPIWindow.h"

// WINDOWS
#include "Application/Windows/WindowsAPIWindow.h"

// OPENGL
#include "GraphicAPI/OpenGL/OpenGLArrayBuffer.h"
#include "GraphicAPI/OpenGL/OpenGLBuffer.h"
#include "GraphicAPI/OpenGL/OpenGLShader.h"
#include "GraphicAPI/OpenGL/OpenGLTexture.h"

// DIRECTX
#include "GraphicAPI/DirectX/DirectXRenderAPI.h"
#include "GraphicAPI/DirectX/DirectXArrayBuffer.h"
#include "GraphicAPI/DirectX/DirectXBuffer.h"
#include "GraphicAPI/DirectX/DirectXShader.h"

namespace Crow {
	namespace Platform {

		enum GraphicAPI {
			OPENGL,
			DIRECTX		// Directx 12
		};

		enum ApplicationAPI {
			GLFW,		// Multiplatform - Supports OpenGL, Vulkan & Metal
			WINDOWS		// Windows only - Supports DirectX
		};

		class PlatformAPI {
		public:
			static GraphicAPI s_GraphicAPI;
			static ApplicationAPI s_ApplicationAPI;

			static void GraphicAPIInit(GraphicAPI grahpicApi); // Set all callbacks to right api
			static void ApplicationAPIInit(ApplicationAPI appApi); // Choses which ApplicationAPI will be used

			static inline Window* (*CreateWindowAPI)(WindowProperties props); // Would have wanted to call the function "CreateWindow"

			static inline std::shared_ptr<VertexBuffer> (*CreateVertexBuffer)(float* vertices, const uint size, const BufferProperties& prop);
			static inline std::shared_ptr<IndexBuffer> (*CreateIndexBuffer)(ulong* indices, const uint count);


			static inline ArrayBuffer* (*CreateArrayBuffer)(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer);

			static inline Shader* (*CreateShaderFromPath)(const char* name, const char* path, const BufferProperties& shaderInput);
			static inline Shader* (*CreateShaderFromSource)(const char* name, std::string& source, const BufferProperties& shaderInput);

			static inline Texture* (*CreateTexture)(const char* path, const TextureProperties& props);

			static inline AbstractRenderAPI* (*CreateRenderAPI)();

		private:
			// Applications
			static void GLFWInit();
			static void WindowsInit();

			// Graphics APIs
			static void OpenGLInit();
			static void DirectXInit();
			
			static void CheckWindowsError();
		};
	}
}