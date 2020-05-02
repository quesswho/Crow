#pragma once
#include "Crow/Common.h"

#ifdef CROW_OGL
	#include "Platform/GraphicAPI/OpenGL/OpenGLBuffer.h"
namespace Crow {
	typedef Crow::Platform::OpenGLVertexBuffer VertexBuffer;
	typedef Crow::Platform::OpenGLIndexBuffer IndexBuffer;
}
#elif defined(CROW_DX11)
	#include "Platform/GraphicAPI/DirectX11/DirectX11Buffer.h"
namespace Crow {
	typedef Platform::DirectX11VertexBuffer VertexBuffer;
	typedef Platform::DirectX11IndexBuffer IndexBuffer;
}
#endif