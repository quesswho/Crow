#pragma once

#include "Crow/Graphics/PostEffect.h"

#include "DirectX11Shader.h"
#include "DirectX11Texture.h"

namespace Crow {
	namespace Platform {

		class DirectX11PostEffect : public PostEffect {

			DirectX11Shader* m_Shader;

			ID3D11RenderTargetView* m_RenderTarget;
			ID3D11ShaderResourceView* m_Texture;
			ID3D11SamplerState* m_TextureProperties;

			ID3D11Buffer* m_VertexBuffer;
			ID3D11Buffer* m_IndexBuffer;

			const uint m_StrideInBytes;
			const uint m_Offset;
		public:
			DirectX11PostEffect(Shader* shader);
			DirectX11PostEffect(const char* path);

			~DirectX11PostEffect() override;

			static PostEffect* CreateDirectX11PostEffectFromShader(Shader* shader) { return new DirectX11PostEffect(shader); }
			static PostEffect* CreateDirectX11PostEffectFromPath(const char* path) { return new DirectX11PostEffect(path); }

			void Bind() const override;

			void Draw() const override;
		private:
			void Init();
		};
	}
}