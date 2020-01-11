#include "DirectX11Texture.h"
#include <stb_image.h>

namespace Crow {
	namespace Platform {

		DirectX11Texture::DirectX11Texture(const char* path, const TextureProperties& props)
			: m_Path(path), m_Index(0)
		{
			Init(props);
		}

		DirectX11Texture::~DirectX11Texture()
		{
			m_Texture->Release();
			m_TextureProperties->Release();
		}

		void DirectX11Texture::Bind() const
		{
			DirectX11RenderAPI::GetDeviceContext()->PSSetShaderResources(m_Index, 1, &m_Texture);
			DirectX11RenderAPI::GetDeviceContext()->PSSetSamplers(0, 1, &m_TextureProperties);
		}

		void DirectX11Texture::Unbind() const
		{}

		void DirectX11Texture::Init(const TextureProperties& props)
		{
			int channels;
			uchar* data = stbi_load(m_Path, &m_Width, &m_Height, &channels, 4);

			if (data)
			{

				D3D11_TEXTURE2D_DESC texDesc;
				texDesc.Width = m_Width;
				texDesc.Height = m_Height;
				texDesc.MipLevels = 1;
				texDesc.ArraySize = 1;
				texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				texDesc.SampleDesc.Count = 1;
				texDesc.SampleDesc.Quality = 0;
				texDesc.Usage = D3D11_USAGE_DEFAULT;
				texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				texDesc.CPUAccessFlags = 0;
				texDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA texData;
				texData.pSysMem = data;
				texData.SysMemPitch = (m_Width * 32 + 7) / 8;
				texData.SysMemSlicePitch = ((m_Width * 32 + 7) / 8) * m_Height;
				
				ID3D11Texture2D* tex = nullptr;
				DirectX11RenderAPI::GetDevice()->CreateTexture2D(&texDesc, &texData, &tex);
				if (tex != NULL)
				{
					D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
					ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
					srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = 1;

					DirectX11RenderAPI::GetDevice()->CreateShaderResourceView(tex, &srvDesc, &m_Texture);
					tex->Release();
				}
			}
			else
			{
				CR_CORE_ERROR("Could not load texture: {}", m_Path);
			}
			stbi_image_free(data);

			D3D11_TEXTURE_ADDRESS_MODE wrapS;
			D3D11_TEXTURE_ADDRESS_MODE wrapT;
			switch (props.m_WrapS)
			{
				case CROW_REPEAT:
					wrapS = D3D11_TEXTURE_ADDRESS_WRAP;
					break;
				case CROW_CLAMP_TO_BORDER:
					wrapS = D3D11_TEXTURE_ADDRESS_BORDER;
					break;
				case CROW_CLAMP_TO_EDGE:
					wrapS = D3D11_TEXTURE_ADDRESS_CLAMP;
					break;
				case CROW_MIRRORED_REPEAT:
					wrapS = D3D11_TEXTURE_ADDRESS_MIRROR;
					break;
				default:
					wrapS = D3D11_TEXTURE_ADDRESS_WRAP;
			}
			switch (props.m_WrapT)
			{
				case CROW_REPEAT:
					wrapT = D3D11_TEXTURE_ADDRESS_WRAP;
					break;
				case CROW_CLAMP_TO_BORDER:
					wrapT = D3D11_TEXTURE_ADDRESS_BORDER;
					break;
				case CROW_CLAMP_TO_EDGE:
					wrapT = D3D11_TEXTURE_ADDRESS_CLAMP;
					break;
				case CROW_MIRRORED_REPEAT:
					wrapT = D3D11_TEXTURE_ADDRESS_MIRROR;
					break;
				default:
					wrapT = D3D11_TEXTURE_ADDRESS_WRAP;
			}

			D3D11_FILTER filter;
			if (props.m_MagFilter == CROW_LINEAR)
			{
				if (props.m_MinFilter == CROW_LINEAR_MIPMAP_NEAREST)
					filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				else if (props.m_MinFilter == CROW_LINEAR || props.m_MinFilter == CROW_LINEAR_MIPMAP_LINEAR)
					filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				else if (props.m_MinFilter == CROW_NEAREST || props.m_MinFilter == CROW_NEAREST_MIPMAP_NEAREST)
					filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
				else if (props.m_MinFilter == CROW_NEAREST_MIPMAP_LINEAR)
					filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
				else
					filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			}
			else if (props.m_MagFilter == CROW_NEAREST)
			{
				if (props.m_MinFilter == CROW_NEAREST || props.m_MinFilter == CROW_NEAREST_MIPMAP_NEAREST)
					filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				else if (props.m_MinFilter == CROW_NEAREST_MIPMAP_LINEAR)
					filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				else if (props.m_MinFilter == CROW_LINEAR_MIPMAP_NEAREST)
					filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
				else if (props.m_MinFilter == CROW_LINEAR || props.m_MinFilter == CROW_LINEAR_MIPMAP_LINEAR)
					filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				else
					filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			}
			else
				filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;


			D3D11_SAMPLER_DESC sampDesc;
			sampDesc.Filter = filter;
			sampDesc.AddressU = wrapS;
			sampDesc.AddressV = wrapT;
			sampDesc.AddressW = wrapS;
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
		}
	}
}