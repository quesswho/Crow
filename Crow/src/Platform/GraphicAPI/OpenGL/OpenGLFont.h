#pragma once
#include "Crow/Math/Maths.h"
#include "OpenGLShader.h"
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

		class OpenGLFont {
		private:
			OpenGLShader* m_Shader;
			uint m_VertexBuffer;
			uint m_ArrayBuffer;
			OpenGLCharacter m_Characters[255];
		public:
			OpenGLFont(const char* path);
			OpenGLFont(const char* path, OpenGLShader* shader);
			~OpenGLFont();

			static OpenGLFont* Create(const char* name) { return new OpenGLFont(name); }
			static OpenGLFont* Create(const char* name, OpenGLShader* shader) { return new OpenGLFont(name, shader); }

			void Reload(const char* path);

			void DrawDynamic(const char* text, Math::Vec2 pos, const uint size) const;
		private:
			void LoadCharacters(const char* path);
		};
	}
}