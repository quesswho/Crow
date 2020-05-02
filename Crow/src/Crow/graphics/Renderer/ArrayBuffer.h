#pragma once
#include "Crow/Common.h"

#ifdef CROW_OGL
	#include "Platform/GraphicAPI/OpenGL/OpenGLArrayBuffer.h"
namespace Crow { typedef Crow::Platform::OpenGLArrayBuffer ArrayBuffer; }
#elif defined(CROW_DX11)
	#include "Platform/GraphicAPI/DirectX11/DirectX11ArrayBuffer.h"
namespace Crow { typedef Crow::Platform::DirectX11ArrayBuffer ArrayBuffer; }
#endif

