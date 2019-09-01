#pragma once
#include "Crow/Common.h"

#include <glad/glad.h>

namespace Crow {

	class RenderAPI {

	public:
		static inline void Clear() { glClear(GL_COLOR_BUFFER_BIT); }
		static inline void ClearColor(float r, float g, float b) { glClearColor(r, g, b, 0.0f); }
		static inline void SetViewPort(uint width, uint height) { glViewport(0, 0, width, height); }
		static inline void DrawIndices(uint count) { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0); }
	};
}