#include "ShaderManager.h"

namespace Crow {

	std::unordered_map<const char*, Shader*> ShaderManager::s_Shaders;


	ShaderManager::~ShaderManager()
	{
		//delete Shader*
		std::for_each(s_Shaders.begin(), s_Shaders.end(), [](auto& shader) {
			delete shader.second;
			}
		);


		s_Shaders.clear();
	}
	void ShaderManager::PushShader(Shader* shader)
	{
		s_Shaders[shader->GetName()] = shader;
	}

	Shader* ShaderManager::GetShader(const char* name)
	{
		if (s_Shaders.find(name) != s_Shaders.end()) // if name exists
		{
			return s_Shaders[name];
		}
		else
		{
			CR_CORE_ERROR("Shader not found in ShaderManager: {}", name);
			return nullptr;
		}
	}

	void ShaderManager::PopShader(const char* name)
	{
		if (s_Shaders.find(name) != s_Shaders.end()) // if name exists
		{
			s_Shaders.erase(name);
		}
		else
		{
			CR_CORE_WARNING("Trying to remove a shader that does not exist! {}", name);
		}
		

	}

	void ShaderManager::Clear() 
	{ 
		s_Shaders.clear(); 
		CR_CORE_WARNING("Shaders cleared from ShaderManager");
	}
}