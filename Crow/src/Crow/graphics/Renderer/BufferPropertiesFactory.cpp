#include "BufferPropertiesFactory.h"

namespace Crow {

	static const BufferProperties s_Vertex = {
			{ 3 } // Vertex
	};

	static const BufferProperties s_VertexUv = {
				{ 3 }, // Vertex
				{ 2 }  // Uvs
	};

	static const BufferProperties s_VertexColor = {
				{ 3 }, // Vertex
				{ 3 }  // Color
	};

	BufferProperties BufferPropertiesFactory::Vertex() { return s_Vertex; }
	BufferProperties BufferPropertiesFactory::VertexUv() { return s_VertexUv; }
	BufferProperties BufferPropertiesFactory::VertexColor() { return s_VertexColor; }
}