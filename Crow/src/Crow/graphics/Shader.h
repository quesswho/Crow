#pragma once
#include "Crow/Common.h"

#ifdef CROW_OGL
	#include "Platform/GraphicAPI/OpenGL/OpenGLShader.h"
namespace Crow { typedef Crow::Platform::OpenGLShader Shader; }
#elif defined(CROW_DX11)
	#include "Platform/GraphicAPI/DirectX11/DirectX11Shader.h"
namespace Crow { typedef Crow::Platform::DirectX11Shader Shader; }
#endif