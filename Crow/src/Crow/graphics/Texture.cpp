#include "Texture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Crow {

	Texture::Texture(const char* path, const TextureProperties& props)
		: m_Path(path), m_Index(0), m_Width(0), m_Height(0) // Default texture binds to GL_TEXTURE0
	{
		Init(props);
	}


	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture::Init(const TextureProperties& props)
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
				CR_GAME_FATAL("File format not supported!"); // TODO: Handle this with an error texture
			}

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			CR_GAME_FATAL("Could not load texture: {}", m_Path); // TODO: Handle this with an error texture
		}
		stbi_image_free(data);
	}

	void Texture::Bind() const
	{
		glActiveTexture(GL_TEXTURE0 + m_Index);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::Unbind() const
	{
		glActiveTexture(GL_TEXTURE0 + m_Index);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}