#pragma once
#include "Crow/Common.h"

#include <initializer_list>
#include <vector>

namespace Crow {

	struct BufferElement {

		BufferElement(const char* name, const uint componentCount)
			: m_Name(name), m_ComponentCount(componentCount), m_Offset(0), m_Index(0)
		{}

		inline const uint GetComponentCount() const { return m_ComponentCount; }
		inline const char* GetName() const { return m_Name; }

		inline void SetOffset(const ushort offset) { m_Offset = offset; }
		inline const ushort GetOffset() const { return m_Offset; }


		inline void SetIndex(const uint index) { m_Index = index; }
		inline const uint GetIndex() const { return m_Index; }


	private:
		const char* m_Name;
		const uint m_ComponentCount;
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
				m_Elements[i].SetIndex(i);
				m_Elements[i].SetOffset(m_Stride);
				m_Stride += m_Elements[i].GetComponentCount();
			}
		}

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		ushort m_Stride;
		std::vector<BufferElement> m_Elements;
	private:
	};

}