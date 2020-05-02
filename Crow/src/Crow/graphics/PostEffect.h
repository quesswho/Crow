#pragma once
#include "Crow/Common.h"

#ifdef CROW_OGL
#include "Platform/GraphicAPI/OpenGL/OpenGLPostEffect.h"
namespace Crow { typedef Crow::Platform::OpenGLPostEffect PostEffect; }
#elif defined(CROW_DX11)
#include "Platform/GraphicAPI/DirectX11/DirectX11PostEffect.h"
namespace Crow { typedef Crow::Platform::DirectX11PostEffect PostEffect; }
#endif