#pragma once
#include "Crow/Graphics/Font/Font.h"

namespace Crow {
	namespace Platform {

		struct OpenGLCharacter {
			OpenGLCharacter()
				: m_Texture(0), m_Offset(0)
			{}
			uint m_Texture;
			Math::TVec2<uint> m_Size;
			Math::TVec2<int> m_TextureOffset;
			uint m_Offset;
		};

		class OpenGLFont : public Font {
		private:
		private:
			Shader* m_Shader;
			uint m_VertexBuffer;
			uint m_ArrayBuffer;
			OpenGLCharacter m_Characters[255];
		public:
			OpenGLFont(const char* path);
			OpenGLFont(const char* path, Shader* shader);
			~OpenGLFont() override;

			static Font* CreateOpenGLFont(const char* name) { return new OpenGLFont(name); }
			static Font* CreateOpenGLFont(const char* name, Shader* shader) { return new OpenGLFont(name, shader); }

			void Reload(const char* path) override;

			void DrawDynamic(const char* text, Math::Vec2 pos, const uint size) const override;
		private:
			void LoadCharacters(const char* path);
		};
	}
}