#include "ShaderManager.h"

namespace Crow {

	std::vector<Shader*> ShaderManager::s_Shaders;


	ShaderManager::~ShaderManager()
	{
		for (const auto shader : s_Shaders)
			delete shader;
	}
	void ShaderManager::PushShader(Shader* shader)
	{
		s_Shaders.push_back(shader);
	}

	Shader* ShaderManager::GetShader(const char* name)
	{
		for (const auto shader : s_Shaders)
		{
			if (shader->GetName() == name)
				return shader;
		}
	}

	void ShaderManager::PopShader(Shader* shader)
	{
		for (int i = 0; i < s_Shaders.size(); i++) // int variable because of index when erasing
		{
			if (s_Shaders[i]->GetName() == shader->GetName())
			{
				s_Shaders.erase(s_Shaders.begin() + i);
				break;
			}
		}

	}

	void ShaderManager::Clear() 
	{ 
		s_Shaders.clear(); 
		CR_CORE_WARNING("Shaders cleared from ShaderManager");
	}
}