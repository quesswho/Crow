#include "Renderer2D.h"
#include "Crow/Graphics/Renderer/RenderAPI.h"
#include "Crow/Application.h"

#include "Crow/Graphics/Renderer/BufferPropertiesFactory.h"

namespace Crow {

	Renderer2D::Renderer2D() 
		: m_Size(0), m_Count(0), m_BatchShader(0), m_BatchTexture(0)
	{
		Application::GetAPI()->EnableAlpha();
	}

	Renderer2D::~Renderer2D()
	{
		for (Object2D* renderable : m_Renderables)
		{
			delete renderable;
		}
	}

	void Renderer2D::Begin()
	{}

	void Renderer2D::Flush()
	{
		for (Object2D* batched : m_BatchedObjects)
		{
			batched->Bind();
			Application::GetAPI()->DrawIndices(batched->GetCount());
			delete batched;
		}

		for (auto renderable = m_Renderables.begin(); renderable != m_Renderables.end(); ++renderable)
		{
			(*renderable)->Bind();
			Application::GetAPI()->DrawIndices((*renderable)->GetCount());
		}


		m_Renderables.clear();
		m_BatchedObjects.clear();
	}

	void Renderer2D::BatchSubmit(Batchable2D* renderable)
	{

		for (uint i = 0; i < renderable->m_Count; i++)
		{
			m_BatchIndices.push_back(renderable->m_Indices[i] + m_Batch.size());
		}
		m_Count += renderable->m_Count;

		VertexData vertex;
		for (uint i = 0; i < renderable->m_Size / sizeof(float); i+=5) // stride 5 because vertex = 3, UV = 2
		{
			vertex.x = renderable->m_Vertices[i] + renderable->m_Position.x;
			vertex.y = renderable->m_Vertices[i + 1] + renderable->m_Position.y;
			vertex.z = renderable->m_Vertices[i + 2] + renderable->m_Position.z;
			
			vertex.u = renderable->m_Vertices[i + 3];
			vertex.v = renderable->m_Vertices[i + 4];

			m_Batch.push_back(vertex);
		}

		m_Size += renderable->m_Size;

		m_BatchShader = renderable->m_Shader;
		m_BatchTexture = renderable->m_Texture;
		if (m_Count > 100000) // 100000 vertices for each drawcall
		{
			/*m_BatchedObjects.push_back(new Object2D(PipelineStateObject::Create(
				VertexBuffer::Create((char*) &m_Batch[0], m_Size, m_BatchTexture == NULL ? BufferPropertiesFactory::VertexColor() : BufferPropertiesFactory::VertexUv()),
				IndexBuffer::Create((char*) &m_BatchIndices[0], m_Count),
				m_BatchShader)));*/

			m_Batch.clear();
			m_BatchIndices.clear();
			m_Count = 0;
			m_Size = 0;
		}
	}

	void Renderer2D::End()
	{
		/*m_BatchedObjects.push_back(new Object2D(PipelineStateObject::Create(
			VertexBuffer::Create((char*)& m_Batch[0], m_Size, m_BatchTexture == NULL ? BufferPropertiesFactory::VertexColor() : BufferPropertiesFactory::VertexUv()),
			IndexBuffer::Create((char*)& m_BatchIndices[0], m_Count),
			m_BatchShader)));*/

		m_Batch.clear();
		m_BatchIndices.clear();
		m_Count = 0;
		m_Size = 0;
	}

	void Renderer2D::Submit(Object2D* renderable) 
	{ 
		m_Renderables.push_back(renderable);
	}

	void Renderer2D::Submit(std::vector<Object2D*> other)
	{
		m_Renderables.insert(m_Renderables.end(), other.begin(), other.end());
	}
}