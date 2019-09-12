#pragma once

#include "Crow/Graphics/Texture.h"

namespace Crow {
	namespace Platform {

		class OpenGLTexture : public Texture {
		private:
			uint m_TextureID;
		public:
			OpenGLTexture(const char* path, const TextureProperties& props);
			~OpenGLTexture() override;

			static Texture* CreateOpenGLTexture(const char* path, const TextureProperties& props) { return new OpenGLTexture(path, props); }

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