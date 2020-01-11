#pragma once

#include "Crow/Graphics/Texture.h"

#include "DirectX11RenderAPI.h"

namespace Crow {
	namespace Platform {

		class DirectX11Texture : public Texture {
		private:
			ID3D11ShaderResourceView* m_Texture;
			ID3D11SamplerState* m_TextureProperties;
		public:
			DirectX11Texture(const char* path, const TextureProperties& props);
			~DirectX11Texture() override;

			static Texture* CreateDirectX11Texture(const char* path, const TextureProperties& props) { return new DirectX11Texture(path, props); }

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual const inline int GetWidth() const override { return m_Width; }
			virtual const inline int GetHeight() const override { return m_Height; }
			virtual const inline int GetIndex() const override { return m_Index; }

			virtual inline void SetIndex(const int index) override { m_Index = index; }

		private:
			void Init(const TextureProperties& props);

			int m_Index, m_Width, m_Height;
			const char* m_Path;
		};
	}
}