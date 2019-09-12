#include "ShaderFactory.h"

namespace Crow {

	static const char* s_ColorShader =
#include "Shaders/Color.glsl"
		;

	static const char* s_TextureShader =
#include "Shaders/Texture.glsl"
		;

	std::string ShaderFactory::ColorShader() { return std::string(s_ColorShader); }
	std::string ShaderFactory::TextureShader() { return std::string(s_TextureShader); }
}