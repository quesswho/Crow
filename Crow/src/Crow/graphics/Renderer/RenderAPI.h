#pragma once
#include "Crow/Common.h"
#include "Crow/Graphics/Window.h"

#include "Crow/Graphics/ShaderFactory.h"

namespace Crow {


	class AbstractRenderAPI {
	public:
		virtual ~AbstractRenderAPI() = default;

		static AbstractRenderAPI* Create();

		virtual bool InitAPI(const WindowProperties& winpropm, void* windowHandle) const = 0;
		virtual void EndInit() const = 0;

		virtual void Begin() const = 0;
		virtual void End() const = 0;

		virtual inline void ClearColor(float r, float g, float b) const = 0;
		virtual inline void SetViewPort(uint width, uint height) const = 0;
		virtual inline void DrawIndices(uint count) const = 0;
		virtual inline void EnableBlending() const = 0;
		virtual inline void EnableDepthTest() const = 0;
		virtual inline void EnableStencilTest() const = 0;

		virtual inline const char* GetAPIName() const = 0;
		virtual inline ShaderFactory* GetShaderFactory() const = 0;

		virtual inline std::string GetGraphicsInfo() const = 0;
	};
}