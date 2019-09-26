#include "OpenGLRenderAPI.h"

namespace Crow {
	namespace Platform {

		OpenGLRenderAPI::OpenGLRenderAPI()
		{
			m_ShaderFactory = new OpenGLShaderFactory();
		}

		bool OpenGLRenderAPI::InitAPI(const WindowProperties&, void*) const
		{ 
			return gladLoadGL();
		}

		std::string OpenGLRenderAPI::GetGraphicsInfo() const
		{
			std::string result((const char*)glGetString(GL_RENDERER));
			result.append(" : Opengl Version: ").append((const char*)glGetString(GL_VERSION));
			return result;
		}
	}
}