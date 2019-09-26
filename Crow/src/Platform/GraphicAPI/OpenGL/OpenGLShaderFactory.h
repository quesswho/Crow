#pragma once

#include "Crow/Graphics/ShaderFactory.h"

namespace Crow {
	namespace Platform {
		class OpenGLShaderFactory : public ShaderFactory {

			virtual std::string BasicShader() override;
			virtual std::string ColorShader() override;
			virtual std::string TextureShader() override;
		};
	}
}