#pragma once

namespace Axton
{
	class ComponentPool
	{
	public:
		ComponentPool(int size, int count, const std::string& type, const std::string& baseType)
		{
			m_Size = size;
			m_Type = type;
			m_BaseType = baseType;
			m_Data = new char[m_Size * count]; // TODO: [axton] make this dynamic
		}

		~ComponentPool()
		{
			delete[] m_Data;
		}

		void* Get(size_t index)
		{
			return m_Data + index * m_Size;
		}

		bool IsType(const std::string& type)
		{
			return m_Type == type || m_BaseType == type;
		}

	private:
		char* m_Data;
		int m_Size;
		std::string m_Type;
		std::string m_BaseType;
	};
}