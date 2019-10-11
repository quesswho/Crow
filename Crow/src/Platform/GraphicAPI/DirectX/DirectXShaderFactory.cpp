#include "DirectXShaderFactory.h"

namespace Crow {
	namespace Platform {

		static const char* s_BasicShader =
#include "Shaders/Basic.hlsl"
			;

		static const char* s_ColorShader =
#include "Shaders/Color.hlsl"
			;
		static const char* s_CbColorShader =
#include "Shaders/cbColor.hlsl"
			;

		static const char* s_TextureShader =
#include "Shaders/Texture.hlsl"
			;

		std::string DirectXShaderFactory::BasicShader() { return std::string(s_BasicShader); }
		std::string DirectXShaderFactory::ColorShader() { return std::string(s_ColorShader); }
		std::string DirectXShaderFactory::UniformColorShader() { return std::string(s_CbColorShader); }
		std::string DirectXShaderFactory::TextureShader() { return std::string(s_TextureShader); }
	}
}