#include "OpenGLArrayBuffer.h"

#include <glad/glad.h>

namespace Crow {
	namespace Platform {

		OpenGLArrayBuffer::OpenGLArrayBuffer(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer)
			: m_VertexBuffer(vBuffer), m_IndexBuffer(iBuffer)
		{
			glGenVertexArrays(1, &m_VertexArrayID);
			glBindVertexArray(m_VertexArrayID);
			iBuffer->Bind();

			if (vBuffer->GetBufferProperties().m_Stride > 0)
			{
				vBuffer->Bind();
				const auto& prop = vBuffer->GetBufferProperties();
				for (const auto& element : prop)
				{
					glEnableVertexAttribArray(element.GetIndex());
					glVertexAttribPointer(element.GetIndex(), element.GetComponentCount(), GL_FLOAT, GL_FALSE, prop.m_Stride * sizeof(float), (const void*)(element.GetOffset() * sizeof(float)));
				}
			}
			else {
				CR_CORE_ERROR("Vertex buffer is missing Properties!");
			}
		}

		OpenGLArrayBuffer::~OpenGLArrayBuffer()
		{
			glDeleteVertexArrays(1, &m_VertexArrayID);
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