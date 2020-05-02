#pragma once

#include "Crow/Math/Maths.h"

#include "DirectX11Shader.h"
#include "dx11.h"

namespace Crow {
	namespace Platform {

		class DirectX11PostEffect {

			DirectX11Shader* m_Shader;

			ID3D11RenderTargetView* m_RenderTarget;
			ID3D11ShaderResourceView* m_Texture;
			ID3D11SamplerState* m_TextureProperties;

			ID3D11Buffer* m_VertexBuffer;
			ID3D11Buffer* m_IndexBuffer;

			const uint m_StrideInBytes;
			const uint m_Offset;
		public:
			DirectX11PostEffect(DirectX11Shader* shader);
			DirectX11PostEffect(const char* path);

			~DirectX11PostEffect();

			static DirectX11PostEffect* CreateFromShader(DirectX11Shader* shader) { return new DirectX11PostEffect(shader); }
			static DirectX11PostEffect* CreateFromPath(const char* path) { return new DirectX11PostEffect(path); }

			void Bind() const;

			void Draw() const;
		private:
			void Init();
		};
	}
}