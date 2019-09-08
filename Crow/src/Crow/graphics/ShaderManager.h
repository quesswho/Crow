#pragma once

#include "Shader.h"
#include <vector>

namespace Crow {

	struct ShaderManager {

		~ShaderManager();

		static void PushShader(Shader* shader);
		static Shader* GetShader(const char* name);

		static void PopShader(const char* name);
		static void PopShader(Shader* shader) { PopShader(shader->GetName()); }

		static void Clear();

	private:
		static std::unordered_map<const char*, Shader*> s_Shaders;
	};
}