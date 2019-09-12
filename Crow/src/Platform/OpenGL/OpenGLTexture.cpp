#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Crow {
	namespace Platform {

		OpenGLTexture::OpenGLTexture(const char* path, const TextureProperties& props)
			: m_Path(path), m_Index(0), m_Width(0), m_Height(0) // Default texture binds to GL_TEXTURE0
		{
			Init(props);
		}


		OpenGLTexture::~OpenGLTexture()
		{
			glDeleteTextures(1, &m_TextureID);
		}

		void OpenGLTexture::Init(const TextureProperties& props)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, props.m_WrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, props.m_WrapT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, props.m_MinFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, props.m_MagFilter);

			int  channels;
			stbi_set_flip_vertically_on_load(true);
			uchar* data = stbi_load(m_Path, &m_Width, &m_Height, &channels, 0);
			if (data)
			{

				int internalFormat = 0;
				switch (channels)
				{
				case 3:
					internalFormat = GL_RGB;
					break;
				case 4:
					internalFormat = GL_RGBA;
					break;
				default:
					CR_CORE_FATAL("File format not supported!"); // TODO: Handle this with an error texture
				}

				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				CR_CORE_FATAL("Could not load texture: {}", m_Path); // TODO: Handle this with an error texture
			}
			stbi_image_free(data);
		}

		void OpenGLTexture::Bind() const
		{
			glActiveTexture(GL_TEXTURE0 + m_Index);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
		}

		void OpenGLTexture::Unbind() const
		{
			glActiveTexture(GL_TEXTURE0 + m_Index);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}