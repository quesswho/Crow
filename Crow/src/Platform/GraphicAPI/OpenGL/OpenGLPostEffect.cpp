#include "OpenGLPostEffect.h"
#include "OpenGLShader.h"

#include "Crow/FileUtils.h"
#include "Crow/Application.h"

#include <glad/glad.h>
#include <regex>
#include <sstream>

namespace Crow {
	namespace Platform {

		OpenGLPostEffect::OpenGLPostEffect(OpenGLShader* shader)
			: m_Shader(static_cast<OpenGLShader*>(shader)->GetHandle())
		{
			Init();
		}

		OpenGLPostEffect::OpenGLPostEffect(const char* path)
		{
			CompileShader(path);
			Init();
		}

		OpenGLPostEffect::~OpenGLPostEffect()
		{
			glDeleteProgram(m_Shader);
			glDeleteTextures(1, &m_Texture);
			glDeleteRenderbuffers(1, &m_RenderBuffer);
			glDeleteFramebuffers(1, &m_FrameBuffer);
			glDeleteVertexArrays(1, &m_VertexArray);
		}

		void OpenGLPostEffect::Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
			glEnable(GL_DEPTH_TEST); // Keep this maybe?
		}

		void OpenGLPostEffect::Draw() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(m_Shader);
			glBindVertexArray(m_VertexArray);
			glBindTexture(GL_TEXTURE_2D, m_Texture);
			glActiveTexture(GL_TEXTURE0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		void OpenGLPostEffect::Init()
		{
			glUseProgram(m_Shader);
			glUniform1i(glGetUniformLocation(m_Shader, "u_Texture"), 0);

			float screenQuad[] = {
				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f,  1.0f,  1.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f
			};
			
			uint vbuffer;
			glGenVertexArrays(1, &m_VertexArray);
			glGenBuffers(1, &vbuffer);
			glBindVertexArray(m_VertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), &screenQuad, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			glGenFramebuffers(1, &m_FrameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

			glGenTextures(1, &m_Texture);
			glBindTexture(GL_TEXTURE_2D, m_Texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Application::s_WindowProperties.m_Width, Application::s_WindowProperties.m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

			glGenRenderbuffers(1, &m_RenderBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Application::s_WindowProperties.m_Width, Application::s_WindowProperties.m_Height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				CR_CORE_ERROR("Failed to create a frame buffer");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// TODO: Make some shader compiler class or something instead of this
		void OpenGLPostEffect::CompileShader(const char* path)
		{
			OpenGLShader::ShaderType type = OpenGLShader::ShaderType::UNKNOWN;

			std::string stringFragmentSource = "";
			std::string stringVertexSource = "";

			std::istringstream source(FileUtils::ReadFile(path));
			std::string line;
			while (std::getline(source, line))
			{
				if (line == "#shader fragment")
				{
					type = OpenGLShader::ShaderType::FRAGMENT;
					continue;
				}

				if (line == "#shader vertex")
				{
					type = OpenGLShader::ShaderType::VERTEX;
					continue;
				}

				switch (type) {
					case OpenGLShader::ShaderType::FRAGMENT:
						stringFragmentSource.append(line).append("\n");
						break;
					case OpenGLShader::ShaderType::VERTEX:
						stringVertexSource.append(line).append("\n");
						break;
				}
			}

			if (stringFragmentSource == "" || stringVertexSource == "")
			{
				CR_CORE_ERROR("Post processing shader source is not in a Crow Shader format! Use \"#shader fragment\" or \"#shader vertex\" to differentiate between fragment shader code and vertex shader code!");
				return;
			}

			const char* vertex = stringVertexSource.c_str();
			const char* fragment = stringFragmentSource.c_str();

			uint vertexID = glCreateShader(GL_VERTEX_SHADER);

			glShaderSource(vertexID, 1, &vertex, NULL);
			glCompileShader(vertexID);

			int success;
			char infoLog[512];
			bool failed = false;
			glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
				std::string stringlog = "Vertex Shader: ";
				stringlog.append(infoLog);
				stringlog = std::regex_replace(stringlog, std::regex("\\ERROR:"), "");
				CR_CORE_ERROR(stringlog);
				failed = true;
			}

			//FRAGMENT

			uint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(fragmentID, 1, &fragment, NULL);
			glCompileShader(fragmentID);

			glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
				std::string stringlog = "Fragment Shader: ";
				stringlog.append(infoLog);
				stringlog = std::regex_replace(stringlog, std::regex("\\ERROR:"), "");
				CR_CORE_ERROR(stringlog);
				failed = true;
			}


			if (failed) return; // Get error from both shaders

			//LINKING

			m_Shader = glCreateProgram();
			glAttachShader(m_Shader, vertexID);
			glAttachShader(m_Shader, fragmentID);
			glLinkProgram(m_Shader);

			glGetProgramiv(m_Shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_Shader, 512, NULL, infoLog);
				std::string stringlog = infoLog;
				stringlog.append("Linking Shader: ");
				stringlog = std::regex_replace(stringlog, std::regex("\\Error:"), "");
				CR_CORE_ERROR(stringlog);
				return;
			}
			glDeleteShader(vertexID);
			glDeleteShader(fragmentID);
		}
	}
}