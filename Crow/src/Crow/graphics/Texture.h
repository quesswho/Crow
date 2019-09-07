#pragma once

#define CROW_NEAREST 0x2600
#define CROW_LINEAR 0x2601
#define CROW_NEAREST_MIPMAP_NEAREST 0x2700
#define CROW_LINEAR_MIPMAP_NEAREST 0x2701
#define CROW_NEAREST_MIPMAP_LINEAR 0x2702
#define CROW_LINEAR_MIPMAP_LINEAR 0x2703

#define CROW_REPEAT 0x2901
#define CROW_MIRRORED_REPEAT 0x8370
#define CROW_CLAMP_TO_EDGE 0x812F


#include "Crow/Common.h"

namespace Crow {
	
	struct TextureProperties {
		const int m_MagFilter, m_MinFilter, m_WrapS, m_WrapT;

		TextureProperties(const int magFilter = CROW_NEAREST_MIPMAP_LINEAR, const int minFilter = CROW_LINEAR)
			: m_MagFilter(magFilter), m_MinFilter(minFilter), m_WrapS(CROW_REPEAT), m_WrapT(CROW_REPEAT)
		{}

		TextureProperties(const int magFilter, const int minFilter, const int wrapS, const int wrapT)
			: m_MagFilter(magFilter), m_MinFilter(minFilter), m_WrapS(wrapS), m_WrapT(wrapT)
		{}
	};
	
	class Texture {
		private:
			uint m_TextureID;
		public:
			Texture(const char* path, const TextureProperties& props = TextureProperties());
			~Texture();
			
			void Init(const TextureProperties& props);
			
			void Bind() const;
			void Unbind() const;
			
			const inline int GetWidth() const { return m_Width; }
			const inline int GetHeight() const { return m_Height; }
			const inline int GetIndex() const { return m_Index; }
			
			inline void SetIndex(const int index) { m_Index = index; }
			
		private:
			int m_Index, m_Width, m_Height;
			const char* m_Path;
	};
}