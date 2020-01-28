#pragma once

#include "Crow/Graphics/Shader.h"

namespace Crow {

	class Font {
	public:
		virtual ~Font() = default;

		static Font* Create(const char* path);
		static Font* Create(const char* path, Shader* shader);

		virtual void Reload(const char* path) = 0;

		virtual void DrawDynamic(const char* text, Math::Vec2 pos, const uint size) const = 0;
	};
}