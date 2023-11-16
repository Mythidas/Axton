#pragma once

#include <queue>

namespace Axton
{
	template <typename T>
	class Queue
	{
	public:
		void Enque(const T& item);
		T Deque();

		void Clear();
		bool Empty();
		size_t Size();

	private:
		std::queue<T> m_Queue;
	};

	template<typename T>
	inline void Queue<T>::Enque(const T& item)
	{
		m_Queue.push(item);
	}

	template<typename T>
	inline T Queue<T>::Deque()
	{
		T item = m_Queue.front();
		m_Queue.pop();
		return item;
	}

	template<typename T>
	inline void Queue<T>::Clear()
	{
		while (!m_Queue.empty())
		{
			m_Queue.pop();
		}
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