#pragma once

#include "Platform/GraphicAPI/DirectX11/DirectX11Shader.h"

#include "dx11.h"
#include "Crow/Math/Maths.h"

namespace Crow {
	namespace Platform {

		struct DirectX11Character {
			DirectX11Character()
				: m_Texture(0), m_Offset(0)
			{}
			ID3D11ShaderResourceView* m_Texture;
			Math::TVec2<uint> m_Size;
			Math::TVec2<int> m_TextureOffset;
			uint m_Offset;
		};

		class DirectX11Font {
		private:
			DirectX11Shader* m_Shader;
			DirectX11Character m_Characters[255];
			ID3D11SamplerState* m_TextureProperties;
			ID3D11Buffer* m_VertexBuffer;
			ID3D11Buffer* m_IndexBuffer;
			const uint m_StrideInBytes;
			const uint m_Offset;
		public:
			DirectX11Font(const char* path);
			DirectX11Font(const char* path, DirectX11Shader* shader);
			~DirectX11Font();

			static DirectX11Font* Create(const char* name) { return new DirectX11Font(name); }
			static DirectX11Font* Create(const char* name, DirectX11Shader* shader) { return new DirectX11Font(name, shader); }

			void Reload(const char* path);

			void DrawDynamic(const char* text, Math::Vec2 pos, const uint size) const;
		private:
			void LoadCharacters(const char* path);
		};
	}
}