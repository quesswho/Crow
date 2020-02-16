#pragma once

#include "Shader.h"

namespace Crow {

	struct PostEffect
	{
		static PostEffect* CreateFromShader(Shader* shader);
		static PostEffect* CreateFromPath(const char* path);

		virtual ~PostEffect() = default;

		virtual void Bind() const = 0;

		virtual void Draw() const = 0;
	};
}