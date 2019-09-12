#include "OpenGLArrayBuffer.h"

#include <glad/glad.h>

namespace Crow {
	namespace Platform {

		OpenGLArrayBuffer::OpenGLArrayBuffer()
		{
			glGenVertexArrays(1, &m_VertexArrayID);
		}

		OpenGLArrayBuffer::~OpenGLArrayBuffer()
		{
			glDeleteVertexArrays(1, &m_VertexArrayID);
		}

		void OpenGLArrayBuffer::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
		{
			if (vertexBuffer->GetBufferProperties().m_Stride > 0)
			{
				glBindVertexArray(m_VertexArrayID);
				vertexBuffer->Bind();
				const auto& prop = vertexBuffer->GetBufferProperties();
				for (const auto& element : prop)
				{
					glEnableVertexAttribArray(element.getIndex());
					glVertexAttribPointer(element.getIndex(), element.getTypeSize(), GL_FLOAT, GL_FALSE, prop.m_Stride * sizeof(GL_FLOAT), (const void*)(element.getOffset() * sizeof(GL_FLOAT)));
				}
				m_VertexBuffers.push_back(vertexBuffer);

			}
			else {
				CR_CORE_ERROR("Vertex buffer is missing Properties!");
			}
		}

		void OpenGLArrayBuffer::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
		{
			glBindVertexArray(m_VertexArrayID);
			indexBuffer->Bind();
			m_IndexBuffer = indexBuffer;
		}

		void OpenGLArrayBuffer::Bind() const
		{
			glBindVertexArray(m_VertexArrayID);
		}

		void OpenGLArrayBuffer::Unbind() const
		{
			glBindVertexArray(0);
		}
	}
}