#include "ArrayBuffer.h"

#include <glad/glad.h>

namespace Crow {

	ArrayBuffer::ArrayBuffer()
	{
		glGenVertexArrays(1, &m_VertexArrayID);
	}

	ArrayBuffer::~ArrayBuffer()
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void ArrayBuffer::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		if (vertexBuffer->GetBufferProperties().m_Stride > 0)
		{
			glBindVertexArray(m_VertexArrayID);
			vertexBuffer->Bind();
			const auto& prop = vertexBuffer->GetBufferProperties();
			for (const auto& element : prop)
			{
				glEnableVertexAttribArray(element.getIndex());
				glVertexAttribPointer(element.getIndex(), element.getTypeSize(), GL_FLOAT, GL_FALSE, prop.m_Stride * sizeof(GL_FLOAT), (const void*)element.getOffset());
				m_VertexBuffers.push_back(vertexBuffer);
			}

		}
		else {
			CR_GAME_ERROR("Vertex buffer is missing Properties!");
		}
	}

	void ArrayBuffer::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_VertexArrayID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

	void ArrayBuffer::Bind() const
	{
		glBindVertexArray(m_VertexArrayID);
	}

	void ArrayBuffer::Unbind() const
	{
		glBindVertexArray(0);
	}
}