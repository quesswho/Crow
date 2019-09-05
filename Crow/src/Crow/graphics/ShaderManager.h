#pragma once

#include "Shader.h"
#include <vector>

namespace Crow {

	struct ShaderManager {

		~ShaderManager();

		static void PushShader(Shader* shader);
		static Shader* GetShader(const char* name);

		static void PopShader(Shader* shader);

		static void Clear();

	private:
		static std::vector<Shader*> s_Shaders;
	};
}