#include "DirectX11Font.h"
#include "DirectX11RenderAPI.h"
#include "Crow/Application.h"

namespace Crow {
	namespace Platform {

		DirectX11Font::DirectX11Font(const char* path)
			: m_StrideInBytes(4 * 4), m_Offset(0)
		{
			BufferProperties bufferprop = {
				{ "POSITION", 2 },
				{ "TEXCOORD", 2 }
			};

			m_Shader = DirectX11Shader::CreateFromSource("BasicFont", Application::GetAPI()->GetShaderFactory()->BasicFontShader(), bufferprop);
			LoadCharacters(path);
		}

		DirectX11Font::DirectX11Font(const char* path, DirectX11Shader* shader)
			: m_Shader(shader), m_StrideInBytes(4 * 4), m_Offset(0)
		{
			LoadCharacters(path);
		}

		DirectX11Font::~DirectX11Font()
		{
			for (int i = 0; i < 255; i++)
			{
				m_Characters[i].m_Texture->Release();
			}
			m_TextureProperties->Release();
			m_VertexBuffer->Release();
			delete m_Shader;
		}

		void DirectX11Font::LoadCharacters(const char* path)
		{

			FT_Face face;
			if (FT_New_Face(Application::s_FreeTypeLibrary, path, 0, &face))
				CR_CORE_ERROR("Failed to load font: {0}", path);

			FT_Set_Pixel_Sizes(face, 0, 48);

			for (uchar c = 0; c < 255; c++)
			{

				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					CR_CORE_ERROR("Failed to load character '{0}'", c);
				}

				D3D11_TEXTURE2D_DESC texDesc;
				texDesc.Width = face->glyph->bitmap.width;
				texDesc.Height = face->glyph->bitmap.rows;
				texDesc.MipLevels = 1;
				texDesc.ArraySize = 1;
				texDesc.Format = DXGI_FORMAT_R8_UNORM;
				texDesc.SampleDesc.Count = 1;
				texDesc.SampleDesc.Quality = 0;
				texDesc.Usage = D3D11_USAGE_DEFAULT;
				texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				texDesc.CPUAccessFlags = 0;
				texDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA texData;
				texData.pSysMem = face->glyph->bitmap.buffer;
				texData.SysMemPitch = (face->glyph->bitmap.width * 8 + 7) / 8;
				texData.SysMemSlicePitch = ((face->glyph->bitmap.width * 8 + 7) / 8) * face->glyph->bitmap.rows;

				ID3D11Texture2D* tex = nullptr;
				DirectX11RenderAPI::GetDevice()->CreateTexture2D(&texDesc, &texData, &tex);
				if (tex != NULL)
				{
					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
					srvDesc.Format = DXGI_FORMAT_R8_UNORM;
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = 1;

					DirectX11RenderAPI::GetDevice()->CreateShaderResourceView(tex, &srvDesc, &m_Characters[c].m_Texture);
					tex->Release();
				}

				m_Characters[c].m_Offset = face->glyph->advance.x;
				m_Characters[c].m_Size = Math::TVec2<uint>(face->glyph->bitmap.width, face->glyph->bitmap.rows);
				m_Characters[c].m_TextureOffset = Math::TVec2<int>(face->glyph->bitmap_left, face->glyph->bitmap_top);
			}

			FT_Done_Face(face);

			D3D11_SAMPLER_DESC sampDesc;
			sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.MipLODBias = 0.0f;
			sampDesc.MaxAnisotropy = 0;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.BorderColor[0] = 0;
			sampDesc.BorderColor[1] = 0;
			sampDesc.BorderColor[2] = 0;
			sampDesc.BorderColor[3] = 0;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

			DirectX11RenderAPI::GetDevice()->CreateSamplerState(&sampDesc, &m_TextureProperties);

			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));

			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.ByteWidth = sizeof(float) * 4 * 4;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vertexData = {};
			ZeroMemory(&vertexData, sizeof(vertexData));

			float allocmem[24];
			ZeroMemory(&allocmem, sizeof(allocmem));

			vertexData.pSysMem = allocmem;

			DirectX11RenderAPI::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer);

			ZeroMemory(&bufferDesc, sizeof(bufferDesc));

			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.ByteWidth = sizeof(float) * 6;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA indexData;
			uint indices[] = {
				0,1,2,1,3,2 
			};
			indexData.pSysMem = reinterpret_cast<BYTE*>(indices);
			DirectX11RenderAPI::GetDevice()->CreateBuffer(&bufferDesc, &indexData, &m_IndexBuffer);

			
		}

		void DirectX11Font::Reload(const char* path)
		{
			LoadCharacters(path);
		}

		void DirectX11Font::DrawDynamic(const char* text, Math::Vec2 pos, const uint size) const
		{
			DirectX11RenderAPI::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			m_Shader->Bind();
			m_Shader->SetUniformValue("u_Projection", Math::Mat4::Orthographic(0.0f, (float) Application::s_WindowProperties.m_Width, 0.0f, (float) Application::s_WindowProperties.m_Height));
			m_Shader->SetUniformValue("u_Color", Math::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			

			DirectX11RenderAPI::GetDeviceContext()->PSSetSamplers(0, 1, &m_TextureProperties);

			D3D11_MAPPED_SUBRESOURCE mappedResource;

			const char* c = &text[0];
			while (*c != '\0') {
				if (*c == ' ')
				{
					pos.x += 22 * size;
					++c;
					continue;
				}

				DirectX11Character ch = m_Characters[*c];

				float x = pos.x + ch.m_TextureOffset.x * size;
				float y = pos.y - (ch.m_Size.y - ch.m_TextureOffset.y) * size;

				float w = (float) ch.m_Size.x * size;
				float h = (float) ch.m_Size.y * size;

				float vertices[] = {
					x + w, y + h,   1.0f, 0.0f,
					x, y + h,		0.0f, 0.0f,
					x + w, y,		1.0f, 1.0f,
					x, y,			0.0f, 1.0f
				};

				DirectX11RenderAPI::GetDeviceContext()->PSSetShaderResources(0, 1, &ch.m_Texture);

				ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
				DirectX11RenderAPI::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, vertices, sizeof(vertices));
				DirectX11RenderAPI::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

				DirectX11RenderAPI::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_StrideInBytes, &m_Offset);

				DirectX11RenderAPI::GetDeviceContext()->DrawIndexed(6, 0, 0);

				pos.x += (ch.m_Offset >> 6) * size;

				++c;
			}
		}
	}
}