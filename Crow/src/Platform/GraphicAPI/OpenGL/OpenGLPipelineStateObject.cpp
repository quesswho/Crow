#include "OpenGLPipelineStateObject.h"

#include <glad/glad.h>

namespace Crow {
	namespace Platform {

		OpenGLPipelineStateObject::OpenGLPipelineStateObject(const std::shared_ptr<VertexBuffer> vBuffer, const std::shared_ptr<IndexBuffer> iBuffer, Shader* shader)
			: m_VertexBuffer(vBuffer), m_IndexBuffer(iBuffer), m_Shader(shader)
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
					glVertexAttribPointer(element.GetIndex(), element.GetComponentCount(), GL_FLOAT, GL_FALSE, prop.m_Stride * sizeof(GL_FLOAT), (const void*)(element.GetOffset() * sizeof(GL_FLOAT)));
				}
			}
			else {
				CR_CORE_ERROR("Vertex buffer is missing Properties!");
			}
		}

		OpenGLPipelineStateObject::~OpenGLPipelineStateObject()
		{
			glDeleteVertexArrays(1, &m_VertexArrayID);
		}

		void OpenGLPipelineStateObject::Bind() const
		{
			m_Shader->Bind();
			glBindVertexArray(m_VertexArrayID);
		}

		void OpenGLPipelineStateObject::Unbind() const
		{
			glBindVertexArray(0);
		}
	}
}