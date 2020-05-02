#pragma once
#include "Crow/Common.h"

#ifdef CROW_OGL
#include "Platform/GraphicAPI/OpenGL/OpenGLTexture.h"
namespace Crow { typedef Crow::Platform::OpenGLTexture Texture; }
#elif defined(CROW_DX11)
#include "Platform/GraphicAPI/DirectX11/DirectX11Texture.h"
namespace Crow { typedef Crow::Platform::DirectX11Texture Texture; }
#endif