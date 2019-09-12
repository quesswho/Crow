#pragma once
#include "Crow/Common.h"

namespace Crow {


	class AbstractRenderAPI {
	public:
		virtual ~AbstractRenderAPI() = default;

		static AbstractRenderAPI* Create();

		virtual bool InitAPI() const = 0;

		virtual inline void Clear() const = 0;
		virtual inline void ClearColor(float r, float g, float b) const = 0;
		virtual inline void SetViewPort(uint width, uint height) const = 0;
		virtual inline void DrawIndices(uint count) const = 0;

		virtual inline const char* GetAPIName() const = 0;
		virtual inline std::string GetGraphicsInfo() const = 0;
	};
}