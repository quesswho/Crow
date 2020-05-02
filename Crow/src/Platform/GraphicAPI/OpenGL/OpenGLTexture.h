#pragma once
#include "Crow/Common.h"

#include "Crow/Graphics/TextureProp.h"

namespace Crow {
	namespace Platform {

		class OpenGLTexture {
		private:
			uint m_TextureID;
		public:
			OpenGLTexture(const char* path, const TextureProperties& props);
			~OpenGLTexture();

			static OpenGLTexture* Create(const char* path, const TextureProperties& props) { return new OpenGLTexture(path, props); }

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