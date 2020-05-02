#pragma once
#include "OpenGLShader.h"

namespace Crow {
	namespace Platform {

		class OpenGLPostEffect {
			uint m_Shader;
			uint m_FrameBuffer;
			uint m_RenderBuffer;
			uint m_Texture;
			uint m_VertexArray;
		public:
			OpenGLPostEffect(OpenGLShader* shader);
			OpenGLPostEffect(const char* path);

			~OpenGLPostEffect();

			static OpenGLPostEffect* CreateFromShader(OpenGLShader* shader) { return new OpenGLPostEffect(shader); }
			static OpenGLPostEffect* CreateFromPath(const char* path) { return new OpenGLPostEffect(path); }

			void Bind() const;

			void Draw() const;
		private:
			void Init();
			void CompileShader(const char* path);
		};
	}
}