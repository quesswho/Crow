#pragma once

#include "Shader.h"

namespace Crow {

	struct ShaderFactory {

		virtual std::string BasicShader() = 0;
		virtual std::string ColorShader() = 0;
		virtual std::string TextureShader() = 0;
	};
}