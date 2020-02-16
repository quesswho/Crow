#pragma once

#include "Crow/Graphics/PostEffect.h"

namespace Crow {
	namespace Platform {

		class OpenGLPostEffect : public PostEffect {
			uint m_Shader;
			uint m_FrameBuffer;
			uint m_RenderBuffer;
			uint m_Texture;
			uint m_VertexArray;
		public:
			OpenGLPostEffect(Shader* shader);
			OpenGLPostEffect(const char* path);

			~OpenGLPostEffect() override;

			static PostEffect* CreateOpenGLPostEffectFromShader(Shader* shader) { return new OpenGLPostEffect(shader); }
			static PostEffect* CreateOpenGLPostEffectFromPath(const char* path) { return new OpenGLPostEffect(path); }

			void Bind() const override;

			void Draw() const override;
		private:
			void Init();
			void CompileShader(const char* path);
		};
	}
}