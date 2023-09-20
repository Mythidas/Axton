#pragma once

#include <functional>

namespace Axton
{
	template <typename... Args>
	class Event
	{
		typedef std::function<void(Args...)> Func;
	public:
		void operator()(Args... args)
		{
			for (auto& callback : m_Callbacks)
			{
				callback(args...);
			}
		}

		void operator +=(const Func& callback)
		{
			m_Callbacks.push_back(callback);
		}

	private:
		std::vector<Func> m_Callbacks;
	};
}