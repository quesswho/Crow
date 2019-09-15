#pragma once

#include "Object2D.h"

namespace Crow {

	struct VertexData {
		float x, y, z;
		float u, v;
	};



	class Renderer2D
	{
	private:
		std::vector<Object2D*> m_Renderables;
		std::vector<Object2D*> m_BatchedObjects;

		Shader* m_BatchShader;
		Texture* m_BatchTexture;
		std::vector<VertexData> m_Batch;
		int m_Size;
		std::vector<int> m_BatchIndices;
		int m_Count;
	public:

		Renderer2D();

		~Renderer2D();

		void Begin();
		void BatchSubmit(Batchable2D* renderable);
		void Submit(Object2D* renderable);
		void Submit(std::vector<Object2D*> renderables);
		void End();
		void Flush();

		//inline Shader* GetBatchShader() const
	};
}