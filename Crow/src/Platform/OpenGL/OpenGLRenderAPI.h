#pragma once

#include "Crow/Graphics/Renderer/RenderAPI.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Crow {
	namespace Platform {

		class OpenGLRenderAPI : public AbstractRenderAPI {
		public:
			OpenGLRenderAPI() {}
			~OpenGLRenderAPI() override {}

			static AbstractRenderAPI* CreateOpenGLRenderAPI() { return new OpenGLRenderAPI(); }

			virtual bool InitAPI() const override { return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); }

			virtual inline void Clear() const override { glClear(GL_COLOR_BUFFER_BIT); }
			virtual inline void ClearColor(float r, float g, float b) const override { glClearColor(r, g, b, 0.0f); }
			virtual inline void SetViewPort(uint width, uint height) const override { glViewport(0, 0, width, height); }
			virtual inline void DrawIndices(uint count) const override { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0); }

			virtual inline const char* GetAPIName() const override { return "OpenGL"; }

			virtual inline std::string GetGraphicsInfo() const override
			{
				std::string result((const char*)glGetString(GL_RENDERER));
				result.append(" : Opengl Version: ").append((const char*)glGetString(GL_VERSION));
				return result;
			}
		};
	}
}