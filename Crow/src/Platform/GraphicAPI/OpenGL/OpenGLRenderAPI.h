#pragma once

#include "Crow/Graphics/Renderer/RenderAPI.h"

#include <glad/glad.h>

#include "OpenGLShaderFactory.h"

namespace Crow {
	namespace Platform {

		class OpenGLRenderAPI : public AbstractRenderAPI {
		public:
			OpenGLRenderAPI();
			virtual ~OpenGLRenderAPI() override 
			{
				delete m_ShaderFactory;
			}

			static AbstractRenderAPI* CreateOpenGLRenderAPI() { return new OpenGLRenderAPI(); }

			virtual bool InitAPI(const WindowProperties&, void*) const override;
			virtual void EndInit() const override {}

			virtual void Begin() const override { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
			virtual void End() const override {}

			virtual inline void ClearColor(float r, float g, float b) const override { glClearColor(r, g, b, 0.0f); }
			virtual inline void SetViewPort(uint width, uint height) const override { glViewport(0, 0, width, height); }
			virtual inline void DrawIndices(uint count) const override { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0); }

			virtual inline void EnableAlpha() const override { glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND); }
			virtual inline void EnableDepthTest() const override { glEnable(GL_DEPTH_TEST); }

			virtual inline const char* GetAPIName() const override { return "OpenGL"; }
			virtual inline ShaderFactory* GetShaderFactory() const override { return m_ShaderFactory; }

			virtual std::string GetGraphicsInfo() const override;
		private:
			ShaderFactory* m_ShaderFactory;
		};
	}
}