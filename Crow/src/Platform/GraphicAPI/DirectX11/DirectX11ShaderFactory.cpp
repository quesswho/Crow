#include "DirectX11ShaderFactory.h"

namespace Crow {
	namespace Platform {

		static const char* s_InterpolationShader =
#include "Shaders/Interpolation.hlsl"
			;

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

		static const char* s_BasicLightShader =
#include "Shaders/BasicLight.hlsl"
			;

		std::string DirectX11ShaderFactory::InterpolationShader() { return std::string(s_InterpolationShader); }
		std::string DirectX11ShaderFactory::BasicShader() { return std::string(s_BasicShader); }
		std::string DirectX11ShaderFactory::ColorShader() { return std::string(s_ColorShader); }
		std::string DirectX11ShaderFactory::UniformColorShader() { return std::string(s_CbColorShader); }
		std::string DirectX11ShaderFactory::TextureShader() { return std::string(s_TextureShader); }
		std::string DirectX11ShaderFactory::BasicLightShader() { return std::string(s_BasicLightShader); }
	}
}