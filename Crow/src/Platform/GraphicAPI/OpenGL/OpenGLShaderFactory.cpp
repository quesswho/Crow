#include "OpenGLShaderFactory.h"

namespace Crow {
	namespace Platform {

		static const char* s_BasicShader =
#include "Shaders/Basic.glsl"
			;

		static const char* s_ColorShader =
#include "Shaders/Color.glsl"
			;

		static const char* s_UniformColorShader =
#include "Shaders/uColor.glsl"
			;

		static const char* s_TextureShader =
#include "Shaders/Texture.glsl"
			;

		static const char* s_BasicLightShader =
#include "Shaders/BasicLight.glsl"
			;

		std::string OpenGLShaderFactory::BasicShader() { return std::string(s_BasicShader); }
		std::string OpenGLShaderFactory::ColorShader() { return std::string(s_ColorShader); }
		std::string OpenGLShaderFactory::UniformColorShader() { return std::string(s_UniformColorShader); }
		std::string OpenGLShaderFactory::TextureShader() { return std::string(s_TextureShader); }
		std::string OpenGLShaderFactory::BasicLightShader() { return std::string(s_BasicLightShader); }
	}
}