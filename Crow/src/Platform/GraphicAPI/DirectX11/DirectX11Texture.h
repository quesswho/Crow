#pragma once

#include "Crow/Graphics/TextureProp.h"
#include "DirectX11RenderAPI.h"

namespace Crow {
	namespace Platform {

		class DirectX11Texture {
		private:
			ID3D11ShaderResourceView* m_Texture;
			ID3D11SamplerState* m_TextureProperties;
		public:
			DirectX11Texture(const char* path, const TextureProperties& props);
			~DirectX11Texture();

			static DirectX11Texture* Create(const char* path, const TextureProperties& props) { return new DirectX11Texture(path, props); }

			void Bind() const;
			void Unbind() const;

			const inline int GetWidth() const { return m_Width; }
			const inline int GetHeight() const { return m_Height; }
			const inline int GetIndex() const { return m_Index; }

			inline void SetIndex(const int index) { m_Index = index; }

		private:
			void Init(const TextureProperties& props);

			int m_Index, m_Width, m_Height;
			const char* m_Path;
		};
	}
}