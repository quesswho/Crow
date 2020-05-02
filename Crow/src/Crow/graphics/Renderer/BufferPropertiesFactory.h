#pragma once

#include "BufferProp.h"

namespace Crow {

	struct BufferPropertiesFactory {

		static BufferProperties Vertex();
		static BufferProperties VertexUv();
		static BufferProperties VertexColor();
	};
}