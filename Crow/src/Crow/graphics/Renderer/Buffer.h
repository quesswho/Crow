#pragma once
#include "Crow/Common.h"

#include <initializer_list>
#include <vector>

namespace Crow {

	struct BufferElement {

		BufferElement(const uint typeSize)
			: m_TypeSize(typeSize), m_Offset(0), m_Index(0)
		{}

		inline const uint getTypeSize() const { return m_TypeSize; }

		inline void setOffset(const ushort offset) { m_Offset = offset; }
		inline const ushort getOffset() const { return m_Offset; }


		inline void setIndex(const uint index) { m_Index = index; }
		inline const uint getIndex() const { return m_Index; }

	private:
		const uint m_TypeSize;
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
			for (int i = 0; i < m_Elements.size(); i++)
			{
				m_Elements[i].setIndex(i);
				m_Elements[i].setOffset(m_Stride);
				m_Stride += m_Elements[i].getTypeSize();
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
	public:


		virtual ~VertexBuffer() = default;

		static std::shared_ptr<VertexBuffer> Create(const float* vertices, const uint size, const BufferProperties& prop);

		virtual const BufferProperties& GetBufferProperties() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		static std::shared_ptr<IndexBuffer> Create(const uint* indices, const uint count);

		virtual inline const uint GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};

}