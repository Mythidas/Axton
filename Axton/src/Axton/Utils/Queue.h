#pragma once

#include <vector>

namespace Axton
{
	template <typename T>
	class Queue
	{
	public:
		void Enque(const T& item);
		T Deque();
		T& Top();

		void Remove(size_t index);
		void Remove(const T& item);
		void Clear();
		bool Empty();
		size_t Size();

	private:
		std::vector<T> m_Queue;
	};

	template<typename T>
	inline void Queue<T>::Enque(const T& item)
	{
		m_Queue.push_back(item);
	}

	template<typename T>
	inline T Queue<T>::Deque()
	{
		T item = m_Queue.back();
		m_Queue.pop_back();
		return item;
	}

	template<typename T>
	inline T& Queue<T>::Top()
	{
		return m_Queue.back();
	}

	template<typename T>
	inline void Queue<T>::Remove(size_t index)
	{
		m_Queue.erase(std::next(m_Queue.begin(), index));
	}

	template<typename T>
	inline void Queue<T>::Remove(const T& item)
	{
		for (size_t i = 0; i < Size(); i++)
		{
			if (m_Queue[i] == item)
			{
				Remove(i);
				break;
			}
		}
	}

	template<typename T>
	inline void Queue<T>::Clear()
	{
		m_Queue.clear();
	}

	template<typename T>
	inline bool Queue<T>::Empty()
	{
		return m_Queue.empty();
	}

	template<typename T>
	inline size_t Queue<T>::Size()
	{
		return m_Queue.size();;
	}
}