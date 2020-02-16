#include "OpenGLRenderAPI.h"

namespace Crow {
	namespace Platform {

		OpenGLRenderAPI::OpenGLRenderAPI()
		{
			m_ShaderFactory = new OpenGLShaderFactory();
		}

		bool OpenGLRenderAPI::InitAPI(const WindowProperties& props, void*) const
		{ 
			bool result = gladLoadGL();

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CW);

			glViewport(0, 0, props.m_Width, props.m_Height);

			return result;
		}

		std::string OpenGLRenderAPI::GetGraphicsInfo() const
		{
			std::string result((const char*)glGetString(GL_RENDERER));
			result.append(" : Opengl Version: ").append((const char*)glGetString(GL_VERSION));
			return result;
		}
	}
}