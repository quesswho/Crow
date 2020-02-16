#include "Platform/GraphicAPI/OpenGL/OpenGLFont.h"

#include "Crow/Application.h"

namespace Crow {
	namespace Platform {

		OpenGLFont::OpenGLFont(const char* path)
		{
			BufferProperties bufferprop = {
				{ "POSITION", 4 }
			};

			m_Shader = Shader::CreateFromSource("BasicFont", Application::GetAPI()->GetShaderFactory()->BasicFontShader(), bufferprop);
			m_Shader->Bind();
			m_Shader->SetUniformValue("u_Texture", 0);
			m_Shader->SetUniformValue("u_Color", Math::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			m_Shader->SetUniformValue("u_Projection", Math::Mat4::OrthographicRH(0.0f, (float)Application::s_WindowProperties.m_Width, 0.0f, (float)Application::s_WindowProperties.m_Height));

			LoadCharacters(path);
		}

		OpenGLFont::OpenGLFont(const char* path, Shader* shader)
			: m_Shader(shader)
		{
			LoadCharacters(path);
		}

		OpenGLFont::~OpenGLFont()
		{
			for (int i = 0; i < 255; i++)
			{
				glDeleteTextures(1, &m_Characters[i].m_Texture);
			}

			glDeleteBuffers(1, &m_VertexBuffer);
			glDeleteVertexArrays(1, &m_ArrayBuffer);
			delete m_Shader;
		}

		void OpenGLFont::LoadCharacters(const char* path)
		{

			FT_Face face;
			if (FT_New_Face(Application::s_FreeTypeLibrary, path, 0, &face))
				CR_CORE_ERROR("Failed to load font: {0}", path);

			FT_Set_Pixel_Sizes(face, 0, 48);
			
			// Use 1 byte packing
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			for (uchar c = 0; c < 255; c++)
			{
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					CR_CORE_ERROR("Failed to load character '{0}'", c);
				}

				glGenTextures(1, &m_Characters[c].m_Texture);
				glBindTexture(GL_TEXTURE_2D, m_Characters[c].m_Texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				m_Characters[c].m_Offset = face->glyph->advance.x;
				m_Characters[c].m_Size = Math::TVec2<uint>(face->glyph->bitmap.width, face->glyph->bitmap.rows);
				m_Characters[c].m_TextureOffset = Math::TVec2<int>(face->glyph->bitmap_left, face->glyph->bitmap_top);
			}
			glBindTexture(GL_TEXTURE_2D, 0);

			FT_Done_Face(face);


			glGenVertexArrays(1, &m_ArrayBuffer);
			glGenBuffers(1, &m_VertexBuffer);
			glBindVertexArray(m_ArrayBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void OpenGLFont::Reload(const char* path)
		{
			LoadCharacters(path);
		}

		void OpenGLFont::DrawDynamic(const char* text, Math::Vec2 pos, const uint size) const
		{
			m_Shader->Bind();
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(m_ArrayBuffer);

			const char* c = &text[0];
			while (*c != '\0') {
				if (*c == ' ')
				{
					pos.x += 22 * size;
					++c;
					continue;
				}

				OpenGLCharacter ch = m_Characters[*c];

				float x = pos.x + ch.m_TextureOffset.x * size;
				float y = pos.y - (ch.m_Size.y - ch.m_TextureOffset.y) * size;

				float w = (float) ch.m_Size.x * size;
				float h = (float) ch.m_Size.y * size;
				
				float vertices[6][4] = {
					{ x,     y + h,   0.0, 0.0 },
					{ x + w, y,       1.0, 1.0 },
					{ x,     y,       0.0, 1.0 },

					{ x,     y + h,   0.0, 0.0 },
					{ x + w, y + h,   1.0, 0.0 },
					{ x + w, y,       1.0, 1.0 }
				};

				glBindTexture(GL_TEXTURE_2D, ch.m_Texture);
				glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				pos.x += (ch.m_Offset >> 6) * size;

				++c;
			}
		}
	}
}