#pragma once
#include "Crow/Common.h"

#include <initializer_list>
#include <vector>

namespace Crow {

	struct BufferElement {

		BufferElement(const uint typeSize, const char* name)
			: m_TypeSize(typeSize), m_Name(name), m_Offset(0), m_Index(0)
		{}

		inline const uint getTypeSize() const { return m_TypeSize; }

		inline void setOffset(const ushort offset) { m_Offset = offset; }
		inline const ushort getOffset() const { return m_Offset; }


		inline void setIndex(const uint index) { m_Index = index; }
		inline const uint getIndex() const { return m_Index; }

	private:
		const char* m_Name;
		uint m_TypeSize;
		uint m_Index;
		ushort m_Offset;
		
	};

	struct BufferProperties
	{
		BufferProperties()
			: m_Stride(0)
		{}

		BufferProperties(std::initializer_list<BufferElement> elements)
			: m_Elements(elements), m_Stride(0)
		{
			int i = 0;
			for (BufferElement& el : m_Elements)
			{
				el.setIndex(i);
				el.setOffset(m_Stride);
				m_Stride += el.getTypeSize();
				i++;
			}
		}

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		ushort m_Stride;
	private:
		std::vector<BufferElement> m_Elements;
	};

	class VertexBuffer {
	private:
		uint m_BufferID;
	public:
		VertexBuffer(const float* vertices, const uint size, const BufferProperties& prop);
		~VertexBuffer();

		const BufferProperties& GetBufferProperties() const { return m_BufferProperties; }

		void Bind() const;
		void Unbind() const;
	private:
		const BufferProperties m_BufferProperties;
	};

	class IndexBuffer {
	private:
		uint m_BufferID;

	public:
		IndexBuffer(const uint* indices, const uint count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline const uint GetCount() { return m_Count; }
	private:
		const uint m_Count;

	};

}