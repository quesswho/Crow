#pragma once

#ifdef CROW_OGL
#include "Platform/GraphicAPI/OpenGL/OpenGLShaderFactory.h"
namespace Crow { typedef Crow::Platform::OpenGLShaderFactory ShaderFactory; }
#elif defined(CROW_DX11)
#include "Platform/GraphicAPI/DirectX11/DirectX11ShaderFactory.h"
namespace Crow { typedef Crow::Platform::DirectX11ShaderFactory ShaderFactory; }
#endif