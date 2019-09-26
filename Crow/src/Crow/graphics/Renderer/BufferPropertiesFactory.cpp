#include "BufferPropertiesFactory.h"

namespace Crow {

	static const BufferProperties s_Vertex = {
			{ "POSITION", 3 } // Vertex
	};

	static const BufferProperties s_VertexUv = {
				{"POSITION", 3 }, // Vertex
				{ "UV", 2 }  // Uvs
	};

	static const BufferProperties s_VertexColor = {
				{ "POSITION", 3 }, // Vertex
				{ "COLOR", 3 }  // Color
	};

	BufferProperties BufferPropertiesFactory::Vertex() { return s_Vertex; }
	BufferProperties BufferPropertiesFactory::VertexUv() { return s_VertexUv; }
	BufferProperties BufferPropertiesFactory::VertexColor() { return s_VertexColor; }
}