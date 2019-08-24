#pragma once
#include "Crow/Common.h"


namespace Crow {

	class CROW_API RenderAPI {

	public:
		static inline void Clear() { glClear(GL_COLOR_BUFFER_BIT); }
		//static inline void ClearColor() { glClearColor() }
		static inline void SetViewPort(unsigned int width, unsigned int height) { glViewport(0, 0, width, height); }
	};
}