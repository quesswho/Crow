#pragma once
#include "Crow/Common.h"

#include "Crow/Graphics/Window.h"
#include "OpenGLShaderFactory.h"

#include <glad/glad.h>

namespace Crow {
	namespace Platform {

		class OpenGLRenderAPI {
		public:
			OpenGLRenderAPI();
			~OpenGLRenderAPI() 
			{
				delete m_ShaderFactory;
			}

			static OpenGLRenderAPI* Create() { return new OpenGLRenderAPI(); }

			bool InitAPI(const WindowProperties&, void*) const;
			void EndInit() const {}

			void Begin() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }
			void End() const {}

			inline void ClearColor(float r, float g, float b) const { glClearColor(r, g, b, 0.0f); }
			inline void SetViewPort(uint width, uint height) const { glViewport(0, 0, width, height); }
			inline void DrawIndices(uint count) const { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0); }

			inline void EnableBlending() const { glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND); }
			inline void EnableDepthTest() const { glEnable(GL_DEPTH_TEST); }
			inline void EnableStencilTest() const { glEnable(GL_STENCIL_TEST); }

			inline const char* GetAPIName() const { return "OpenGL"; }
			inline OpenGLShaderFactory* GetShaderFactory() const { return m_ShaderFactory; }

			std::string GetGraphicsInfo() const;
		private:
			OpenGLShaderFactory* m_ShaderFactory;
		};
	}
}