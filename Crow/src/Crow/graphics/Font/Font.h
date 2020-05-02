#pragma once
#include "Crow/Common.h"

#ifdef CROW_OGL
#include "Platform/GraphicAPI/OpenGL/OpenGLFont.h"
namespace Crow { typedef Crow::Platform::OpenGLFont Font; }
#elif defined(CROW_DX11)
#include "Platform/GraphicAPI/DirectX11/DirectX11Font.h"
namespace Crow { typedef Crow::Platform::DirectX11Font Font; }
#endif