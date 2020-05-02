#pragma once
#include <string>

namespace Crow {
	namespace Platform {
		class OpenGLShaderFactory {
		public:
			std::string InterpolationShader();
			std::string BasicShader();
			std::string ColorShader();
			std::string UniformColorShader();
			std::string TextureShader();
			std::string BasicLightShader();

			std::string BasicFontShader();
		};
	}
}