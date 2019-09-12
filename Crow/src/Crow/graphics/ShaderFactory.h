#pragma once

#include "Shader.h"

namespace Crow {

	struct ShaderFactory {
		static std::string ColorShader();
		static std::string TextureShader(); 
	};
}