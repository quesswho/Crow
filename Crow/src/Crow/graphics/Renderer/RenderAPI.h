#pragma once
#include "Crow/Common.h"

#ifdef CROW_OGL
#include "Platform/GraphicAPI/OpenGL/OpenGLRenderAPI.h"
namespace Crow { typedef Crow::Platform::OpenGLRenderAPI RenderAPI; }
#elif defined(CROW_DX11)
#include "Platform/GraphicAPI/DirectX11/DirectX11RenderAPI.h"
namespace Crow { typedef Crow::Platform::DirectX11RenderAPI RenderAPI; }
#endif