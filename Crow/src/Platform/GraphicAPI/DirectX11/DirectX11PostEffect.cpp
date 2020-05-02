#include "DirectX11PostEffect.h"
#include "DirectX11RenderAPI.h"
#include "DirectX11Shader.h"
#include "Crow/FileUtils.h"

#include "Crow/Application.h"

#include <glad/glad.h>
#include <regex>
#include <sstream>

namespace Crow {
	namespace Platform {

		DirectX11PostEffect::DirectX11PostEffect(DirectX11Shader* shader)
			: m_Shader(static_cast<DirectX11Shader*>(shader)), m_StrideInBytes(16), m_Offset(0)
		{
			Init();
		}

		DirectX11PostEffect::DirectX11PostEffect(const char* path)
			: m_StrideInBytes(16), m_Offset(0)
		{
			m_Shader = static_cast<DirectX11Shader*>(DirectX11Shader::CreateFromPath("Nameless PostFX", path, { { "POSITION", 2 }, { "TEXCOORD", 2 } }));
			Init();
		}

		DirectX11PostEffect::~DirectX11PostEffect()
		{
			delete m_Shader;
			m_RenderTarget->Release();
			m_Texture->Release();
			m_TextureProperties->Release();
			m_VertexBuffer->Release();
			m_IndexBuffer->Release();
		}

		void DirectX11PostEffect::Bind() const
		{
			DirectX11RenderAPI::SetRenderTarget(&m_RenderTarget);
			Application::GetAPI()->SetViewPort(1080,720);
			DirectX11RenderAPI::ClearRenderTarget(m_RenderTarget);
		}

		void DirectX11PostEffect::Draw() const
		{
			DirectX11RenderAPI::SetRenderTarget(0);

			m_Shader->Bind();
			DirectX11RenderAPI::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_StrideInBytes, &m_Offset);
			DirectX11RenderAPI::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			
			DirectX11RenderAPI::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
			DirectX11RenderAPI::GetDeviceContext()->PSSetSamplers(0, 1, &m_TextureProperties);

			DirectX11RenderAPI::GetDeviceContext()->DrawIndexed(6, 0, 0);
		}

		void DirectX11PostEffect::Init()
		{
			float vertices[] = {
					-1.0f, -1.0f, 0.0f, 1.0f,
					-1.0f, 1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 0.0f,
					1.0f, -1.0f, 1.0f, 1.0f
			};

			ulong indices[] = {
				0,2,1,0,3,2
			};

			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));

			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.ByteWidth = sizeof(vertices);
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA vertexData = {};
			ZeroMemory(&vertexData, sizeof(vertexData));

			vertexData.pSysMem = reinterpret_cast<BYTE*>(vertices);
			DirectX11RenderAPI::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer);

			ZeroMemory(&bufferDesc, sizeof(bufferDesc));

			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.ByteWidth = sizeof(indices);
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA indexData;
			indexData.pSysMem = reinterpret_cast<BYTE*>(indices);
			DirectX11RenderAPI::GetDevice()->CreateBuffer(&bufferDesc, &indexData, &m_IndexBuffer);
			DirectX11RenderAPI::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			D3D11_TEXTURE2D_DESC texDesc;
			texDesc.Width = Application::s_WindowProperties.m_Width;
			texDesc.Height = Application::s_WindowProperties.m_Height;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = 0;

			ID3D11Texture2D* tex = nullptr;
			DirectX11RenderAPI::GetDevice()->CreateTexture2D(&texDesc, NULL, &tex);
			if (tex)
			{
				D3D11_SAMPLER_DESC sampDesc;
				sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
				sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
				sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
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

				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
				ZeroMemory(&rtvDesc, sizeof(CD3D11_RENDER_TARGET_VIEW_DESC));
				rtvDesc.Format = texDesc.Format;
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				rtvDesc.Texture2D.MipSlice = 0;

				DirectX11RenderAPI::GetDevice()->CreateRenderTargetView(tex, &rtvDesc, &m_RenderTarget);

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				srvDesc.Format = texDesc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = 1;

				DirectX11RenderAPI::GetDevice()->CreateShaderResourceView(tex, &srvDesc, &m_Texture);
			}
		}
	}
}