#pragma once
#include "Crow/Graphics/Font/Font.h"

#include "DirectX11RenderAPI.h"

namespace Crow {
	namespace Platform {

		struct DirectX11Character {
			ID3D11ShaderResourceView* m_Texture;
			Math::TVec2<int> m_Size;
			Math::TVec2<int> m_TextureOffset;
			uint m_Offset;
		};

		class DirectX11Font : public Font {
		private:
			Shader* m_Shader;
			DirectX11Character m_Characters[255];
			ID3D11SamplerState* m_TextureProperties;
			ID3D11Buffer* m_VertexBuffer;
			ID3D11Buffer* m_IndexBuffer;
			const uint m_StrideInBytes;
			const uint m_Offset;
		public:
			DirectX11Font(const char* path);
			DirectX11Font(const char* path, Shader* shader);
			~DirectX11Font() override;

			static Font* CreateDirectX11Font(const char* name) { return new DirectX11Font(name); }
			static Font* CreateDirectX11Font(const char* name, Shader* shader) { return new DirectX11Font(name, shader); }

			void Reload(const char* path) override;

			void DrawDynamic(const char* text, Math::Vec2 pos, const uint size) const override;
		private:
			void LoadCharacters(const char* path);
		};
	}
}