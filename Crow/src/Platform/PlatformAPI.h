#pragma once

#include "Crow/Graphics/Renderer/Buffer.h"
#include "Crow/Graphics/Renderer/ArrayBuffer.h"


#include "OpenGL/OpenGLRenderAPI.h"

#include "OpenGL/OpenGLBuffer.h"
#include "OpenGL/OpenGLArrayBuffer.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"

namespace Crow {
	namespace Platform {

		enum RenderAPI {
			OPENGL
		};

		class PlatformAPI {
		public:
			static RenderAPI s_RenderAPI;

			static void Init(RenderAPI api); // set all callbacks to right api

			static inline std::shared_ptr<VertexBuffer> (*CreateVertexBuffer)(const float* vertices, const uint size, const BufferProperties& prop);
			static inline std::shared_ptr<IndexBuffer> (*CreateIndexBuffer)(const uint* indices, const uint count);


			static inline ArrayBuffer* (*CreateArrayBuffer)();

			static inline Shader* (*CreateShaderFromPath)(const char* name, const char* path);
			static inline Shader* (*CreateShaderFromSource)(const char* name, std::string& source);

			static inline Texture* (*CreateTexture)(const char* path, const TextureProperties& props);

			static inline AbstractRenderAPI* (*CreateRenderAPI)();

		private:
			static void OpenGLInit();
		};
	}
}